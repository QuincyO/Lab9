#include "MoveToPlayerAction.h"
#include "Enemy.h"
#include <iostream>

MoveToPlayerAction::MoveToPlayerAction(Enemy* agent)
{
	m_name = "MoveToPlayer Action";
	SetAgent(agent);
}

MoveToPlayerAction::~MoveToPlayerAction() = default;

void MoveToPlayerAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	GetAgent()->MoveToPlayer();
}

