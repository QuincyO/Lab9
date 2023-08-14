#pragma once
#ifndef __FLEEACTION_H__
#define __FLEEACTION_H__
#include "ActionNode.h"

class FleeAction : public ActionNode
{
public:
	FleeAction(Enemy* agent);
	virtual ~FleeAction();

	virtual void Action() override;
private:

};

#endif 
