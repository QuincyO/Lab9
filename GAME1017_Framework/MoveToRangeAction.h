#pragma once
#ifndef __MOVETORANGEACTION_H__
#define __MOVETORANGEACTION_H__
#include "ActionNode.h"

class MoveToRangeAction : public ActionNode
{
public:
	MoveToRangeAction(Enemy* agent);
	virtual ~MoveToRangeAction();

	virtual void Action() override;
private:

};

#endif 
