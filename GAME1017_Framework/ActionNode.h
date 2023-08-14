#pragma once
#ifndef __ACTIONNODE_H__
#define __ACTIONNODE_H__
#include "TreeNode.h"

enum class ActionState
{
	IDLE_STATE, PATROL_STATE, MOVETOPLAYER_STATE
};

class ActionNode : public TreeNode // Abstract class.
{
public:
	ActionNode() { m_isLeaf = true; }
	virtual ~ActionNode() = default;
	virtual void Action() = 0;
};

#endif