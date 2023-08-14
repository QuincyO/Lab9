#include "HurtCondition.h"

HurtCondition::HurtCondition(const bool isHurt)
{
	SetIsHurt(isHurt);
	m_name = "Hurt Condition";
}

HurtCondition::~HurtCondition() = default;

void HurtCondition::SetIsHurt(const bool state)
{
	m_isHurt = state;
}

bool HurtCondition::Condition()
{
	return m_isHurt;
}
