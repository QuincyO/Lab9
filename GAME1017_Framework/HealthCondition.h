#pragma once
#ifndef __HEALTHCONDITION_H__
#define __HEALTHCONDITION_H__
#include "ConditionNode.h"

class HealthCondition : public ConditionNode
{
public:
	HealthCondition(bool okHealth = false);
	virtual ~HealthCondition();

	// Getters and Setters
	void SetOkHealth(bool state);

	virtual bool Condition() override;
private:
	bool m_okHealth;
};

#endif