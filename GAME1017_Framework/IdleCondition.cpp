#include "IdleCondition.h"

IdleCondition::IdleCondition(const bool isIdle)
{
	SetIsIdle(isIdle);
	m_name = "Idle Condition";
}

IdleCondition::~IdleCondition() = default;

void IdleCondition::SetIsIdle(const bool state)
{
	m_isIdle = state;
}

bool IdleCondition::Condition()
{
	return m_isIdle;
}
