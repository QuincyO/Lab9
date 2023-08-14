#include "FleeAction.h"
#include "Enemy.h"
#include <iostream>

FleeAction::FleeAction(Enemy* agent)
{
	m_name = "Flee Action";
	SetAgent(agent);
}

FleeAction::~FleeAction() = default;

void FleeAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	//GetAgent()->Flee();
}

