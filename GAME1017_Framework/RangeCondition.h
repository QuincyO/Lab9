#pragma once
#ifndef __RANGECONDITION_H__
#define __RANGECONDITION_H__
#include "ConditionNode.h"

class RangeCondition : public ConditionNode
{
public:
	RangeCondition(bool isWithinRange = false);
	virtual ~RangeCondition();

	// Getters and Setters
	void SetWithinRange(bool state);

	virtual bool Condition() override;
private:
	bool m_isWithinRange;
};

#endif