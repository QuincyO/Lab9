#pragma once
#ifndef __CONDITIONNODE_H__
#define __CONDITIONNODE_H__
#include "TreeNode.h"

class ConditionNode : public TreeNode 
{
public:
	ConditionNode() :m_condition(false) { m_isLeaf = false; }
	virtual ~ConditionNode() = default;
	virtual bool Condition() { return m_condition; };
	void SetCondition(bool state) { m_condition = state; }
private:
	bool m_condition;
};

#endif