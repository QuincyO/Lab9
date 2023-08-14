#include "RangeCondition.h"

RangeCondition::RangeCondition(const bool isWithinRange)
{
	SetWithinRange(isWithinRange);
	m_name = "Range Condition";
}

RangeCondition::~RangeCondition() = default;

void RangeCondition::SetWithinRange(const bool state)
{
	m_isWithinRange = state;
}

bool RangeCondition::Condition()
{
	return m_isWithinRange;
}
