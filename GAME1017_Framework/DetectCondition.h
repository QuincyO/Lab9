#pragma once
#ifndef __DETECTCONDITION_H__
#define __DETECTCONDITION_H__
#include "ConditionNode.h"

class DetectCondition : public ConditionNode
{
public:
	DetectCondition(bool hasDetected = false);
	virtual ~DetectCondition();

	// Getters and Setters
	void SetHasDetected(bool state);

	virtual bool Condition() override;
private:
	bool m_hasDetected;
};

#endif