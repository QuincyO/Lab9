#include "MoveToRangeAction.h"
#include "Enemy.h"
#include <iostream>

MoveToRangeAction::MoveToRangeAction(Enemy* agent)
{
	m_name = "MoveToRange Action";
	SetAgent(agent);
}

MoveToRangeAction::~MoveToRangeAction() = default;

void MoveToRangeAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
	// GetAgent()->MoveToRange();
}

