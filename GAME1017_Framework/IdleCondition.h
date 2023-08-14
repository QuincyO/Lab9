#pragma once
#ifndef __IDLECONDITION_H__
#define __IDLECONDITION_H__
#include "ConditionNode.h"

class IdleCondition : public ConditionNode
{
public:
	IdleCondition(bool isIdle = false);
	virtual ~IdleCondition();

	// Getters and Setters
	void SetIsIdle(bool state);

	virtual bool Condition() override;
private:
	bool m_isIdle;
};

#endif