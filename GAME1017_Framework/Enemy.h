#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameObject.h"
#include "TiledLevel.h" // Could forward declare but TiledLevel.h doesn't include Enemy.h
#include "ActionNode.h" // ActionState in here.

class DecisionTree; // Forward declaration.

class Enemy : public AnimatedSprite
{
public:
	Enemy(SDL_Rect s, SDL_FRect d, TiledLevel* level);
	void Update();
	void Render();
	// Action methods.
	// void Idle(); // No more idle.
	void Patrol();
	void MoveToPlayer();
	DecisionTree* GetTree();
	ActionState GetActionState();
	void SetActionState(ActionState action);
	void BuildTree();
private:
	bool m_dir, m_isPatroling;
	double m_accel,
		m_angle,
		m_dx, m_dy,
		m_vel, m_velMax, m_rotMax;
	SDL_FPoint* m_pTarget; // Target for movement. Used for patrol and movetoPlayer()
	TiledLevel* m_level;
	DecisionTree* m_tree;
	ActionState m_action;
	void StartMove(); // Initialization for movement.
	void MoveToTarget(); // Move increment function.
	void CheckNextWaypoint(); // Waypoint utility. patrol
	void CheckForArrival(); // MoveToPlayer utility. Pathfinding
	void CreatePatrol(); // Patrol creation utility.
	std::vector<SDL_FPoint> m_patrol;
	int m_patrolPoint; //Index of current patrol point
	// For pathfinding.
	int m_pathCtr, m_pathMax, m_pathPoint; //Pathpoint is index of current path point
	std::vector<SDL_FPoint*> m_moveToPath; //Path of FPoint nodes.
	void CreatePath();
};

#endif

