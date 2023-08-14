#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"
#include "TiledLevel.h"

class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, TiledLevel* level);
	void Update();
	void Render();
private:
	bool m_dir;
	int m_updateCounter = 0;
	TiledLevel* m_level;
	bool CheckCollision(const int dX, const int dY);
	
};

#endif

