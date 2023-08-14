#include "TiledLevel.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "MathManager.h"
#include "PathManager.h"
#include "Engine.h"
#include <string>


Tile::Tile(SDL_Rect src, SDL_FRect dst, bool isObs, bool isHaz)
	:Sprite(src, dst), m_obstacle(isObs), m_hazard(isHaz), m_node(nullptr)
{
	m_lCost = m_lX = m_lY = nullptr; // All null Label*.
}

Tile::~Tile()
{
	delete m_node; // Will also invoke PathNode destructor. Deletes connections.
	delete m_lCost;
	delete m_lX;
	delete m_lY;
}

TiledLevel::TiledLevel(const unsigned short rows, const unsigned short cols, const int tileWidth, const int tileHeight, 
	const char* tileData, const char* levelData, const char* tileKey) 
	:m_rows(rows), m_cols(cols), m_tileKey(tileKey), m_showCosts(false)
{
	// First build prototype tiles.
	std::ifstream inFile(tileData);
	if (inFile.is_open()) 
	{
		char key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			m_tiles.emplace(key, new Tile({ x * tileWidth, y * tileHeight, tileWidth, tileHeight },
				{ 0.0f, 0.0f, (float)tileWidth, (float)tileHeight }, obs, haz));
		}
	}
	inFile.close();
	// Now construct the level.
	inFile.open(levelData);
	if (inFile.is_open()) 
	{
		char key;
		m_level.resize(m_rows); // Important or we cannot use subscripts.
		for (unsigned short row = 0; row < m_rows; row++)
		{
			m_level[row].resize(m_cols);
			for (unsigned short col = 0; col < m_cols; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); // Common prototype method.
				m_level[row][col]->SetXY((float)(col * tileWidth), (float)(row * tileHeight));
				// Instantiate the labels for each tile.
				m_level[row][col]->m_lCost = new Label("tile", m_level[row][col]->GetDst()->x + 4, m_level[row][col]->GetDst()->y + 18, " ", { 255,255,255,255 });
				m_level[row][col]->m_lX = new Label("tile", m_level[row][col]->GetDst()->x + 18, m_level[row][col]->GetDst()->y + 2, std::to_string(col).c_str(), { 0,0,0,255 });
				m_level[row][col]->m_lY = new Label("tile", m_level[row][col]->GetDst()->x + 2, m_level[row][col]->GetDst()->y + 2, std::to_string(row).c_str(), { 0,0,0,255 });
				// Add tile to Obstacle vector if impassable.
				if (m_level[row][col]->IsObstacle())
					m_obstacles.push_back(m_level[row][col]);
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard())
					m_level[row][col]->m_node = new PathNode((int)(m_level[row][col]->GetDst()->x), (int)(m_level[row][col]->GetDst()->y));
			}
		}
	}
	inFile.close();
	// Now build the graph from ALL the non-obstacle and non-hazard tiles. Only N-E-W-S compass points.
	for (int row = 0; row < kRows; row++)
	{
		for (int col = 0; col < kCols; col++)
		{
			if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
				continue; // An obstacle or hazard tile has no connections.
			// Make valid connections. Inside map and a valid tile.
			if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row - 1][col]->Node()->x, m_level[row][col]->Node()->y, m_level[row - 1][col]->Node()->y)));
			if (row + 1 != kRows && m_level[row + 1][col]->Node() != nullptr) // Tile below.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row + 1][col]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row + 1][col]->Node()->x, m_level[row][col]->Node()->y, m_level[row + 1][col]->Node()->y)));
			if (col - 1 != -1 && m_level[row][col - 1]->Node() != nullptr) // Tile to Left.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col - 1]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row][col - 1]->Node()->x, m_level[row][col]->Node()->y, m_level[row][col - 1]->Node()->y)));
			if (col + 1 != kCols && m_level[row][col + 1]->Node() != nullptr) // Tile to right.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col + 1]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row][col + 1]->Node()->x, m_level[row][col]->Node()->y, m_level[row][col + 1]->Node()->y)));
		}
	}
}

TiledLevel::~TiledLevel()
{
	// Clear the tile clones. This also clears the ones in m_obstacles.
	for (unsigned short row = 0; row < m_rows; row++)
	{
		for (unsigned short col = 0; col < m_cols; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr;
		}
	}
	m_level.clear();
	m_obstacles.clear();
	// Clear the original tiles.
	for (std::map<char, Tile*>::iterator i = m_tiles.begin(); i != m_tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::Render()
{
	for (unsigned short row = 0; row < m_rows; row++)
	{
		for (unsigned short col = 0; col < m_cols; col++)
		{
			SDL_RenderCopyF(REMA::GetRenderer(), TEMA::GetTexture(m_tileKey),
				m_level[row][col]->GetSrc(), m_level[row][col]->GetDst());
			if (m_showCosts)
			{
				m_level[row][col]->m_lCost->Render();
				m_level[row][col]->m_lX->Render();
				m_level[row][col]->m_lY->Render();
			}			
		}
	}
}

std::vector<Tile*>& TiledLevel::GetObstacles() { return m_obstacles; }

std::vector<std::vector<Tile*>>& TiledLevel::GetLevel() { return m_level; }
