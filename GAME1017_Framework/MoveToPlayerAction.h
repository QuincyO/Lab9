#pragma once
#ifndef __MOVETOPLAYERACTION_H__
#define __MOVETOPLAYERACTION_H__
#include "ActionNode.h"

class MoveToPlayerAction : public ActionNode
{
public:
	MoveToPlayerAction(Enemy* agent);
	virtual ~MoveToPlayerAction();

	virtual void Action() override;
private:

};

#endif 
