#include "PatrolAction.h"
#include "Enemy.h"
#include <iostream>

PatrolAction::PatrolAction(Enemy* agent)
{
	m_name = "Patrol Action";
	SetAgent(agent);
}

PatrolAction::~PatrolAction() = default;

void PatrolAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	GetAgent()->Patrol();
}

