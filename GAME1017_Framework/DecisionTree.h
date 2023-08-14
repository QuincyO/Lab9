#pragma once
#ifndef __DECISIONTREE__
#define __DECISIONTREE__

#include "ConditionNode.h"
//#include "DetectCondition.h"
//#include "HealthCondition.h"
//#include "HurtCondition.h"
//#include "IdleCondition.h"
//#include "LOSCondition.h"
//#include "RangeCondition.h"
#include <vector>

class Enemy; // Forward declaration required.

class DecisionTree
{
public:
	// Constructors and destructor
	DecisionTree();
	DecisionTree(Enemy* agent);
	~DecisionTree();

	// Getters and setters
	Enemy* GetAgent() const;
	std::vector<TreeNode*>& GetNodes();
	void SetAgent(Enemy* agent);
	// Node methods.
	//DetectCondition* GetDetectNode();
	//void SetDetectNode(DetectCondition* node);
	//// Only need Health and Hurt nodes for RangedCombatEnemy
	//HealthCondition* GetHealthNode();
	//void SetHealthNode(HealthCondition* node);
	//HurtCondition* GetHurtNode();
	//void SetHurtNode(HurtCondition* node);
	////
	ConditionNode* GetPatrolNode();
	void SetPatrolNode(ConditionNode* node);
	//LOSCondition* GetLOSNode();
	//void SetLOSNode(LOSCondition* node);
	//RangeCondition* GetRangeNode();
	//void SetRangeNode(RangeCondition* node);
	// Other methods
	TreeNode* AddNode(TreeNode* parent, TreeNode* child, TreeNodeType type);
	void Display();
	void Clean();
	void MakeDecision(); // Traverse tree in order

private:
	Enemy* m_agent;
	std::vector<TreeNode*> m_treeNodeList;
	// Nodes.
	ConditionNode* m_patrolNode;
	/*DetectCondition* m_detectNode;
	ConditionNode* m_healthNode;
	ConditionNode* m_hurtNode;
	ConditionNode* m_idleNode;
	ConditionNode* m_losNode;
	ConditionNode* m_rangeNode;*/
};

#endif