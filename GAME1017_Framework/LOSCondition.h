#pragma once
#ifndef __LOSCONDITION_H__
#define __LOSCONDITION_H__
#include "ConditionNode.h"

class LOSCondition : public ConditionNode
{
public:
	LOSCondition(bool hasLOS = false);
	virtual ~LOSCondition();

	// Getters and Setters
	void SetHasLOS(bool state);

	virtual bool Condition() override;
private:
	bool m_hasLOS;
};

#endif