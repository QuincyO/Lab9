#include "AttackAction.h"
#include "Enemy.h"
#include <iostream>

AttackAction::AttackAction(Enemy* agent)
{
	m_name = "Attack Action";
	SetAgent(agent);
}

AttackAction::~AttackAction() = default;

void AttackAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	// GetAgent()->Attack();
}

