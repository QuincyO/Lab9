#include "LOSCondition.h"

LOSCondition::LOSCondition(const bool hasLOS)
{
	SetHasLOS(hasLOS);
	m_name = "LOS Condition";
}

LOSCondition::~LOSCondition() = default;

void LOSCondition::SetHasLOS(const bool state)
{
	m_hasLOS = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}
