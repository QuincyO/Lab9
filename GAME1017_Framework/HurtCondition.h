#pragma once
#ifndef __HURTCONDITION_H__
#define __HURTCONDITION_H__
#include "ConditionNode.h"

class HurtCondition : public ConditionNode
{
public:
	HurtCondition(bool isHurt = false);
	virtual ~HurtCondition();

	// Getters and Setters
	void SetIsHurt(bool state);

	virtual bool Condition() override;
private:
	bool m_isHurt;
};

#endif