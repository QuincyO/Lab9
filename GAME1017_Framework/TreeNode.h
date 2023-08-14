#pragma once
#ifndef __TREENODE_H__
#define __TREENODE_H__
#include <string>

class Enemy; // Forward declaration required.

enum class TreeNodeType
{
	LEFT_TREE_NODE, RIGHT_TREE_NODE
};

class TreeNode
{
public:
	TreeNode() : m_agent(nullptr) {}
	virtual ~TreeNode() = default;
	Enemy* GetAgent() { return m_agent; }
	void SetAgent(Enemy* agent) { m_agent = agent; }

	bool m_isLeaf = false;
	std::string m_name = "";
	TreeNode* m_pLeft = nullptr;
	TreeNode* m_pRight = nullptr;
	TreeNode* m_pParent = nullptr;

private:
	Enemy* m_agent;
};

#endif