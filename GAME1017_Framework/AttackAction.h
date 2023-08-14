#pragma once
#ifndef __ATTACKACTION_H__
#define __ATTACKACTION_H__
#include "ActionNode.h"

class AttackAction : public ActionNode
{
public:
	AttackAction(Enemy* agent);
	virtual ~AttackAction();

	virtual void Action() override;
private:

};

#endif 
