#include "Enemy.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "Engine.h"
// Managers
#include "StateManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "PathManager.h"
// Decision tree includes.
#include "DecisionTree.h"
#include "IdleCondition.h"
#include "PatrolAction.h"
#include "MoveToPlayerAction.h"
// Others.
#include "TiledLevel.h"
#define SPEED 2.0
#define PFREFRESH 30

Enemy::Enemy(SDL_Rect s, SDL_FRect d, TiledLevel* level)
	:AnimatedSprite(s, d, STATE_IDLING), 
	m_dir(0), m_isPatroling(1), m_level(level), m_velMax(1.0), m_pTarget(nullptr), m_pathCtr(0), m_pathMax(PFREFRESH)
{
	m_accel = m_dx = m_dy = m_vel = m_rotMax = 0.0;
	m_angle = 90.0; // Starting angle. So, facing east to start.
	m_tree = new DecisionTree(this);
	CreatePatrol();
	BuildTree();
	GetTree()->GetPatrolNode()->SetCondition(m_isPatroling);
	SetAnimation(AnimState::STATE_RUNNING, 4, 0, 8, 0);
}

void Enemy::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		m_isPatroling = !m_isPatroling;
		GetTree()->GetPatrolNode()->SetCondition(m_isPatroling);
	}// for lab 9, replace above with below.
	//Do a distance check using a detection range (not set yet), and then set the bool result into the detectNode of tree.
	//GetTree()->GetDetectNode()->SetCondition(Heres where our distance check would go);
	GetTree()->MakeDecision(); // Always per frame, but how can we throttle it?
	Animate();
}

void Enemy::Render()
{
	SDL_RenderCopyExF(REMA::GetRenderer(), TEMA::GetTexture("player"), GetSrc(), GetDst(), 0, 0, static_cast<SDL_RendererFlip>(m_dir));
	// Create LOS whisker. Note the change to GetStates().front() because we can still render when paused.
	SDL_FRect* target = STMA::GetStates().front()->GetChild("player")->GetDst();
	SDL_FPoint targetCenter = STMA::GetStates().front()->GetChild("player")->GetCenter();
	std::vector<Tile*>& obsOrig = STMA::GetStates().front()->GetChild<TiledLevel*>("level")->GetObstacles();
	std::vector<Tile*> obstacles;
	// Add only obstacles that are between player and target.
	double targetDist = MAMA::Distance(GetCenter(), targetCenter);
	for (auto obstacle : obsOrig)
	{
		double obsDist = MAMA::Distance(GetCenter(), obstacle->GetCenter());
		if (obsDist < targetDist) 
			obstacles.push_back(obstacle);
	}
	double angle = MAMA::AngleBetweenPoints(targetCenter.y - GetCenter().y,
		targetCenter.x - GetCenter().x);
	SDL_FPoint end = {GetCenter().x + MAMA::SetDeltaX(angle, 300.0),
	GetCenter().y + MAMA::SetDeltaY(angle, 300.0)};
	if (COMA::LOSCheck(GetCenter(), end, target, obstacles))
		DEMA::DrawLine(GetCenter(), end, { 0,255,0,255 });
	else 
		DEMA::DrawLine(GetCenter(), end, { 255,0,0,255 });
	//This is where were drawwing detection radius, Currently 50 is distance/radius
	DEMA::DrawCircle(GetCenter(), 50.0f, { 255, 255, 0, 255 });
	// New for Lab 9.
	if (GetActionState() == ActionState::MOVETOPLAYER_STATE)
		PAMA::DrawSmoothPath();
}

void Enemy::Patrol()
{
	if (GetActionState() != ActionState::PATROL_STATE)
	{
		SetActionState(ActionState::PATROL_STATE);
		// Set the target to the patrol point using the patrol index.
		// Invoke StartMove().
	}
	// Invoke MoveToTarget().
	// Invoke CheckNextWaypoint().
}

void Enemy::MoveToPlayer()
{
	if (GetActionState() != ActionState::MOVETOPLAYER_STATE) // If current action != the new action.
	{
		SetActionState(ActionState::MOVETOPLAYER_STATE);
		CreatePath();
		StartMove();
		// We'll fill in for Lab 9.
	}
	//Per-Frame checks/actions. Everytime we invokke MakeDecision().
	if (m_pathCtr++ % m_pathMax == 0) //Every 30 Frames we generate a new shortest path
	{
		CreatePath();
	}
	MoveToTarget();
	CheckForArrival(); //Move enemy to next path point or stop at last one.
}

DecisionTree* Enemy::GetTree()
{
	return m_tree;
}

ActionState Enemy::GetActionState()
{
	return m_action;
}

void Enemy::SetActionState(ActionState action)
{
	m_action = action;
}

void Enemy::BuildTree()
{
	// Create and add root node
	ConditionNode* patrolCondition = new ConditionNode();
	m_tree->SetPatrolNode(patrolCondition);
	m_tree->GetNodes().push_back(patrolCondition);
	
	//For one part of lab 9. Keep it as condition node but youre going to detectec condition.

	/*ConditionNode* detectConditien = new ConditionNode();
	m_tree->SetDetectionNode(DetectCondition);
	m_tree->GetNodes().push_back(patrolCondition);

	*/


	TreeNode* patrolAction = m_tree->AddNode(patrolCondition, new PatrolAction(this), TreeNodeType::LEFT_TREE_NODE);
	m_tree->GetNodes().push_back(patrolAction);

	TreeNode* moveToPlayerAction = m_tree->AddNode(patrolCondition, new MoveToPlayerAction(this), TreeNodeType::RIGHT_TREE_NODE);
	m_tree->GetNodes().push_back(moveToPlayerAction);
}

void Enemy::StartMove()
{
	
	// We'll fill in for Lab 9.
	//Set animation to running cycle	
	SetAnimation(AnimState::STATE_RUNNING, 4, 0, 8, 0);
	//Reset some motion Vars, so enemy can move again.
	m_accel = 0.05;
	m_rotMax = 2.0; //This could be in consturctor
	//Set the direction towards the target.

	if (m_pTarget->x < GetCenter().x)
	{
		m_dir = 1;
	}
	else m_dir = 0;

}

void Enemy::MoveToTarget()
{
	// This is taken from Steering example. I'll just go through it in lab.
	double destAngle = 0.0;
	destAngle = MAMA::Rad2Deg(MAMA::AngleBetweenPoints((m_pTarget->y) - GetCenter().y,
		(m_pTarget->x) - GetCenter().x)) + 90;
	m_angle += MAMA::Clamp(MAMA::Angle180(destAngle - m_angle), -m_rotMax, m_rotMax); // Only rotate slightly towards the destination angle.
	m_vel += m_accel;
	m_vel = std::min(m_vel, m_velMax); // Clamp velocity to maximum.
	MAMA::SetDeltas(MAMA::Deg2Rad(m_angle - 90), m_dx, m_dy, m_vel, m_vel);
	// Update various positions.
	m_dst.x += (float)m_dx;
	m_dst.y += (float)m_dy;
}

void Enemy::CheckNextWaypoint()
{
	// Create a temporary SDL_FPoint that is the center of the enemy.
	
	// If distance to target is less than 16
		// Increment patrolPoint
		// If patrolPoint is at the end, reset to 0
		// Set the target to the patrol point using the index
		// Set the direction to the target
}

void Enemy::CheckForArrival()
{
	// Will be filled in for Lab 9.
	m_accel = 0.05;//Reset Accel.
	SDL_FPoint c = GetCenter();
	if (MAMA::Distance(&c, m_pTarget) < 16.0)
	{
		m_pathPoint++; //Increment path point index
		if (m_pathPoint == (int)m_moveToPath.size()) // if were at last path point, we need to stop/
		{
			m_pathPoint = 0;
			m_accel = m_vel = 0.0; //Stops the enemy
			SetAnimation(AnimState::STATE_IDLING, 1, 0, 1);
		}
		m_pTarget = m_moveToPath[m_pathPoint];
		if (m_pTarget->x < GetCenter().x) m_dir = 1;
		else m_dir = 0;
	}
}

void Enemy::CreatePatrol()
{
	// Add the points to the m_patrol vector.
	
	// Set patrolPoint index to 0.
}

void Enemy::CreatePath()
{
	// Will be filled in for Lab 9.
	std::vector<std::vector<Tile*>>& level = STMA::CurrentState()->GetChild<TiledLevel*>("level")->GetLevel();
	//Create temp fields for enenym and player GameObjects
	GameObject* e = STMA::CurrentState()->GetChild("enemy");
	GameObject* p = STMA::CurrentState()->GetChild("player");
	PAMA::GetShortestPath(level[(int)(e->GetCenter().y/32)][(int)(e->GetCenter().x/32)]->Node(), level[(int)(p->GetCenter().y / 32)][(int)(p->GetCenter().x / 32)]->Node());
	for (auto pt : m_moveToPath) //clear out old pathpoints
	{
		delete pt;
		pt = nullptr;

	}
	m_moveToPath.clear();
	m_moveToPath.shrink_to_fit();
	//Now create the movetopath from the smooth path of connections
	for (auto connection : PAMA::GetPath())
	{
		m_moveToPath.push_back(new SDL_FPoint({
			connection->GetToNode()->x + 16.0f,
			connection->GetToNode()->y + 16.0f
			}));
	}
	//If theres no path because enmy is at the same tile as the  player. No path will be generated start = goal
	if (m_moveToPath.size() == 0)
	{
		m_moveToPath.push_back(new SDL_FPoint({
			p->GetCenter()
			}));
	}
	m_pathPoint = 0;
	m_pTarget = m_moveToPath[m_pathPoint];
	if (m_state != AnimState::STATE_RUNNING)
	{
		SetAnimation(AnimState::STATE_RUNNING, 4, 0, 8, 0);
	}

}
