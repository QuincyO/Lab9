#include "DetectCondition.h"

DetectCondition::DetectCondition(const bool hasDetected)
{
	SetHasDetected(hasDetected);
	m_name = "Detect Condition";
}

DetectCondition::~DetectCondition() = default;

void DetectCondition::SetHasDetected(const bool state)
{
	m_hasDetected = state;
}

bool DetectCondition::Condition()
{
	return m_hasDetected;
}
