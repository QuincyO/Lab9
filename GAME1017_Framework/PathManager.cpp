#include "PathManager.h"
#include "DebugManager.h"
#include "MathManager.h"
#include <iostream>
// For Lab 8.
#include "StateManager.h"
#include "TiledLevel.h"

void PathManager::GetShortestPath(PathNode* start, PathNode* goal)
{
	// Clear the old path.
	ClearPath();
	ClearLists();

	if (start == nullptr) return; // If player's location is an obstacle tile, exit. Happens when player can move linearly.
	if (start == goal) return;
	// Initialize the record for the start node.
	NodeRecord* currentRecord = new NodeRecord(start);
	currentRecord->m_totalCost = start->H(); // m_totalCost is F cost, and the G cost for start node is 0.
	// Push the node record for the start node into the open list.
	s_open.push_back(currentRecord);
	
	// Iterate through processing each node.
	while (s_open.size() > 0)
	{
		// Pick the node record with the smallest totalCost. This is the F cost (G + H).
		currentRecord = GetSmallestNode(); 
		// If it is the goal node, then terminate.
		if (currentRecord->m_node == goal)
		{
			s_open.erase(std::remove(s_open.begin(), s_open.end(), currentRecord), s_open.end());
			s_closed.push_back(currentRecord);
			break; // We found the goal, so break out of while loop.
		}
		// Otherwise get its outgoing connections.
		std::vector<PathConnection*> connections = currentRecord->m_node->GetConnections();
		// Loop through each connection in turn.
		for (unsigned i = 0; i < connections.size(); i++)
		{
			// Get the cost estimate for the end node.
			PathNode* endNode = connections[i]->GetToNode();
			// Create a node record pointer for the selections statements below.
			// The node record may exist in a list or we may need to create it if node is undescovered.
			NodeRecord* endNodeRecord;
			// Determine the accumulated connection costs (just G costs) for the new connection.
			// Only the m_totalCost for a record is the F cost (G + H) and it us used to sniff out the smallest node for our informed search.
			double endNodeCost = currentRecord->m_costSoFar + connections[i]->GetCost();

			// If the end node is in the closed list, we may have to skip or remove it from the closed list.
			if (ContainsNode(s_closed, endNode))
			{
				// Here we find the record in the closed list corresponding to the endNode so we can access its cost.
				endNodeRecord = GetNodeRecord(s_closed, endNode);
				// If we didn't find a shorter route, skip.
				if (endNodeRecord->m_costSoFar <= endNodeCost)
					continue;
				// Else we did find a shorter route, so remove the end node from the closed list because it will be added back to the open list.
				s_closed.erase(std::remove(s_closed.begin(), s_closed.end(), endNodeRecord), s_closed.end());
				// And go to the end of the selection block so we can update the pathfinding data and add the node to the open list.
			}
			// If the node is open and we've not found a better route.
			else if (ContainsNode(s_open, endNode))
			{
				// Here we find the record in the open list corresponding to the endNode so we can access its cost.
				endNodeRecord = GetNodeRecord(s_open, endNode);
				// If our route is no better, then skip.
				if (endNodeRecord->m_costSoFar <= endNodeCost)
					continue;
				// Else we did find a shorter route.
				// And go to the end of the selection block so we can update the pathfinding data. Node already in open list though.
			}
			//Otherwise we know we’ve got an unvisited node, so make a record for it.
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->m_node = endNode;
				// And go to the end of the selection block so we can update the pathfinding data and add the node to the open list.
			}
			// We’re here if we need to update the pathfinding data.
			endNodeRecord->m_costSoFar = endNodeCost; // Just G costs so far.
			endNodeRecord->m_connection = connections[i];
			endNodeRecord->m_fromRecord = currentRecord;
			// Update the total cost for a node record, which is the F cost. G + H. This will result in the shortest path possible to goal.
			endNodeRecord->m_totalCost = endNodeCost + endNode->H();
			// And add it to the open list if node isn't already there.
			if (!ContainsNode(s_open, endNode))
				s_open.push_back(endNodeRecord);
		}
		// We’ve finished looking at the connections for the current node, 
		// so remove it from the open list, add it to the closed list.
		s_open.erase(std::remove(s_open.begin(), s_open.end(), currentRecord), s_open.end());
		s_closed.push_back(currentRecord);
	}
	// We’re here if we’ve either found the goal, or if we have no more nodes to search. 
	if (currentRecord->m_node != goal)
	{
		// We’ve run out of nodes without finding the goal, so there’s no solution.
		std::cout << "Could not find path!" << std::endl;
	}
	else // We found the goal, so we now need to create the path of connections.
	{
		// Compile the list of connections in the path, working back along the path, accumulating connections.
		while (currentRecord->m_node != start)
		{
			s_path.push_back(currentRecord->m_connection);
			currentRecord = currentRecord->m_fromRecord;
		}
		// Reverse the path and return it.
		std::reverse(s_path.begin(), s_path.end());
		SmoothPath(); // We got the path, now we smooth it. Draw it in TiledLevel render.
	}
	// Clean up lists. Uncomment the cout below to see how many records we have to clean up.
	// std::cout << "Cleaning up..." << "open size: " << s_open.size() << " | closed size: " << s_closed.size() << std::endl;
	
}

NodeRecord* PathManager::GetSmallestNode()
{
	std::vector<NodeRecord*>::iterator smallest = s_open.begin();
	std::vector<NodeRecord*>::iterator current = s_open.begin();
	
	while (++current != s_open.end())
	{
		if ((*current)->m_totalCost < (*smallest)->m_totalCost)
			smallest = current;
		else if ((*current)->m_totalCost == (*smallest)->m_totalCost) // If equal, flip a coin!
			smallest = (rand() % 2 ? current : smallest);
	}
	return (*smallest);
}

std::vector<NodeRecord*>& PathManager::OpenList()
{
	return s_open;
}

std::vector<NodeRecord*>& PathManager::ClosedList()
{
	return s_closed;
}

bool PathManager::ContainsNode(std::vector<NodeRecord*>& list, PathNode* n)
{
	for (unsigned i = 0; i < list.size(); i++)
		if (list[i]->m_node == n)
			return true;
	return false;
}

NodeRecord* PathManager::GetNodeRecord(std::vector<NodeRecord*>& list, PathNode * n)
{
	for (unsigned i = 0; i < list.size(); i++)
		if (list[i]->m_node == n)
			return list[i];
	return nullptr;
}

double PathManager::HEuclid(const PathNode* start, const PathNode* goal)
{
	return MAMA::Distance(start->x, goal->x, start->y, goal->y); // Shortest distance in pixels. In Pythagorean theorem it's hypotenuse.
}

double PathManager::HManhat(const PathNode * start, const PathNode * goal)
{
	return (std::abs(start->x - goal->x) + std::abs(start->y - goal->y)); // In Pythagorean theorem it's A + B or deltaX + deltaY.
}

void PathManager::DrawPath()
{
	for (unsigned i = 0; i < s_path.size(); i++)
	{
		SDL_Point from = { s_path[i]->GetFromNode()->x + 16, s_path[i]->GetFromNode()->y + 16 };
		SDL_Point to = { s_path[i]->GetToNode()->x + 16, s_path[i]->GetToNode()->y + 16 };
		DEMA::DrawLine(from, to, { 255,255,0,255 });
	}
}

void PathManager::DrawLists()
{
	for (unsigned i = 0; i < s_closed.size(); i++)
	{
		SDL_Rect node = { s_closed[i]->m_node->x + 8, s_closed[i]->m_node->y + 8, 16, 16 };
		DEMA::DrawRect(node, true, { 255,0,0,255 });
	}
	for (unsigned i = 0; i < s_open.size(); i++)
	{
		SDL_Rect node = { s_open[i]->m_node->x + 8, s_open[i]->m_node->y + 8, 16, 16 };
		DEMA::DrawRect(node, true, { 0,0,255,255 });
	}
}

void PathManager::ClearPath()
{
	s_path.clear(); // Clear the old saved path so we can save a new one.
	s_path.shrink_to_fit();
	s_pathSmooth.clear();
	s_pathSmooth.shrink_to_fit();
}

void PathManager::ClearLists()
{
	for (unsigned i = 0; i < s_open.size(); i++)
	{
		delete s_open[i];
		s_open[i] = nullptr; // Wrangle your dangle.
	}
	s_open.clear();
	s_open.shrink_to_fit();
	for (unsigned i = 0; i < s_closed.size(); i++)
	{
		delete s_closed[i];
		s_closed[i] = nullptr; // Wrangle your dangle.
	}
	s_closed.clear();
	s_closed.shrink_to_fit();
}

void PathManager::DrawSmoothPath()
{
	for (unsigned i = 0; i < s_pathSmooth.size(); i++)
	{
		SDL_Point from = { s_pathSmooth[i]->GetFromNode()->x + 16, s_pathSmooth[i]->GetFromNode()->y + 16 };
		SDL_Point to = { s_pathSmooth[i]->GetToNode()->x + 16, s_pathSmooth[i]->GetToNode()->y + 16 };
		DEMA::DrawLine(from, to, { 0,255,255,255 });
	}
}

void PathManager::SmoothPath()
{
	auto from = s_path.begin(); // Using iterators. Like fancy indices.
	auto to = from;
	// Clear the path first.
	// Moved to ClearPath.
	// Define a current connection as the working connection.
	PathConnection* current = new PathConnection((*from)->GetFromNode(), (*from)->GetToNode());
	while (to != s_path.end())
	{
		current->SetToNode((*to)->GetToNode());
		if (CheckPathSegment(current)) // If there's no LOS between from and to nodes.
		{
			// Update current connection's to node, to one node back.
			current->SetToNode((*to)->GetFromNode());
			// Push the new segment into the smooth path vector.
			s_pathSmooth.push_back(current);
			// And now create a new path segment to test.
			from = to;
			current = new PathConnection((*from)->GetFromNode(), (*from)->GetToNode());
		}
		else // There's LOS between from and to nodes.
		{
			// Increment our to iterator to the next path segment.
			to++;
		}
	}
	s_pathSmooth.push_back(current); // Need this so last connection gets into the smoothed list.
}

bool PathManager::CheckPathSegment(PathConnection* c)
{
	for (auto obstacle : STMA::CurrentState()->GetChild<TiledLevel*>("level")->GetObstacles())
	{
		int os = 8; // Offset in pixels from a tile center. Max is 15 or it breaks.
		// We do four separate checks on compass points with an offset from center.
		// From node's points.
		SDL_FPoint fN = { (float)c->GetFromNode()->x + 16, (float)c->GetFromNode()->y + 16 - os }; // North.
		SDL_FPoint fS = { (float)c->GetFromNode()->x + 16, (float)c->GetFromNode()->y + 16 + os }; // South.
		SDL_FPoint fW = { (float)c->GetFromNode()->x + 16 - os, (float)c->GetFromNode()->y + 16 }; // West.
		SDL_FPoint fE = { (float)c->GetFromNode()->x + 16 + os, (float)c->GetFromNode()->y + 16 }; // East.
		// To node's points.
		SDL_FPoint tN = { (float)c->GetToNode()->x + 16, (float)c->GetToNode()->y + 16 - os }; // North.
		SDL_FPoint tS = { (float)c->GetToNode()->x + 16, (float)c->GetToNode()->y + 16 + os }; // South.
		SDL_FPoint tW = { (float)c->GetToNode()->x + 16 - os, (float)c->GetToNode()->y + 16 }; // West.
		SDL_FPoint tE = { (float)c->GetToNode()->x + 16 + os, (float)c->GetToNode()->y + 16 }; // East.
		// Now we do our LOS checks using built-in SDL Line-Rect checks.
		// If any of these if checks pass, it means there's intersection and thus no LOS.
		if (SDL_IntersectFRectAndLine(obstacle->GetDst(), &fN.x, &fN.y, &tN.x, &tN.y)) // North check.
			return true;
		else if (SDL_IntersectFRectAndLine(obstacle->GetDst(), &fS.x, &fS.y, &tS.x, &tS.y)) // South check.
			return true;
		else if (SDL_IntersectFRectAndLine(obstacle->GetDst(), &fW.x, &fW.y, &tW.x, &tW.y)) // West check.
			return true;
		else if (SDL_IntersectFRectAndLine(obstacle->GetDst(), &fE.x, &fE.y, &tE.x, &tE.y)) // East check.
			return true;
	}
	return false; // There is LOS.
}

std::vector<PathConnection*>& PathManager::GetPath()
{
	return s_pathSmooth;
}

// These are necessary definitions for static properties. This is where they get memory.
std::vector<NodeRecord*> PathManager::s_open;
std::vector<NodeRecord*> PathManager::s_closed;

std::vector<PathConnection*> PathManager::s_path;
std::vector<PathConnection*> PathManager::s_pathSmooth;
// For your MoveToPlayer action, you can just have a path of SDL_FPoints.