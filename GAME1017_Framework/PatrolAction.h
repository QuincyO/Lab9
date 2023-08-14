#pragma once
#ifndef __PATROLACTION_H__
#define __PATROLACTION_H__
#include "ActionNode.h"

class PatrolAction : public ActionNode
{
public:
	PatrolAction(Enemy* agent);
	virtual ~PatrolAction();

	virtual void Action() override;
private:

};

#endif 