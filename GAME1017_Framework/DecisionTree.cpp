#include "DecisionTree.h"
#include "ActionNode.h"
#include "ConditionNode.h"
#include "Enemy.h"
#include <iostream>

DecisionTree::DecisionTree() : m_agent(nullptr) {}
DecisionTree::DecisionTree(Enemy* agent) : m_agent(agent) {}

DecisionTree::~DecisionTree() = default;

Enemy* DecisionTree::GetAgent() const
{
	return m_agent;
}

std::vector<TreeNode*>& DecisionTree::GetNodes()
{
	return m_treeNodeList;
}

void DecisionTree::SetAgent(Enemy* agent)
{
	m_agent = agent;
}

//DetectCondition* DecisionTree::GetDetectNode()
//{
//	return m_detectNode;
//}
//
//void DecisionTree::SetDetectNode(DetectCondition* node)
//{
//	m_detectNode = node;
//}

//HealthCondition* DecisionTree::GetHealthNode()
//{
//	return m_healthNode;
//}
//
//void DecisionTree::SetHealthNode(HealthCondition* node)
//{
//	m_healthNode = node;
//}
//
//HurtCondition* DecisionTree::GetHurtNode()
//{
//	return m_hurtNode;
//}
//
//void DecisionTree::SetHurtNode(HurtCondition* node)
//{
//	m_hurtNode = node;
//}

ConditionNode* DecisionTree::GetPatrolNode()
{
	return m_patrolNode;
}

void DecisionTree::SetPatrolNode(ConditionNode* node)
{
	m_patrolNode = node;
}

//LOSCondition* DecisionTree::GetLOSNode()
//{
//	return m_losNode;
//}
//
//void DecisionTree::SetLOSNode(LOSCondition* node)
//{
//	m_losNode = node;
//}
//
//RangeCondition* DecisionTree::GetRangeNode()
//{
//	return m_rangeNode;
//}
//
//void DecisionTree::SetRangeNode(RangeCondition* node)
//{
//	m_rangeNode = node;
//}

TreeNode* DecisionTree::AddNode(TreeNode* parent, TreeNode* child, TreeNodeType type)
{
	switch (type)
	{
	case TreeNodeType::LEFT_TREE_NODE:
		parent->m_pLeft = child;
		break;
	case TreeNodeType::RIGHT_TREE_NODE:
		parent->m_pRight = child;
		break;
	}
	child->m_pParent = parent;
	return child;
}

void DecisionTree::Display()
{
	for (auto node : m_treeNodeList)
		std::cout << node->m_name << std::endl;
}

void DecisionTree::Clean()
{
	for (auto node : m_treeNodeList)
	{
		delete node;
		node = nullptr;
	}
	m_treeNodeList.clear();
	m_treeNodeList.shrink_to_fit();
}

void DecisionTree::MakeDecision()
{
	auto currentNode = m_treeNodeList[0]; // Start at root node
	while (!currentNode->m_isLeaf)
	{
		currentNode = dynamic_cast<ConditionNode*>(currentNode)->Condition() ? (currentNode->m_pLeft) : (currentNode->m_pRight); // Left = true, Right = false.
	}
	static_cast<ActionNode*>(currentNode)->Action(); // Invoke the action node.
}
