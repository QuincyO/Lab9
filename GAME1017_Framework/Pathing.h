#pragma once
#ifndef _PATHING_H_
#define _PATHING_H_
#include <SDL.h>
#include <vector>

class PathConnection; // Forward declaration required.

class PathNode : public SDL_Point // Because why not?
{
public:
	PathNode(int x, int y);
	~PathNode();
	void Update();
	void AddConnection(PathConnection* c);
	std::vector<PathConnection*>& GetConnections();
	SDL_Point Pt() { return { x, y }; }
	double H() { return m_h; } // Getter for cost.
	void SetH(double h) { m_h = h; } // Setter for cost.
private:
	double m_h; // Heuristic cost for node. H cost.
	std::vector<PathConnection*> m_connections; // Max 4 elements.
};

class PathConnection
{
public:
	PathConnection(PathNode* f, PathNode* t, double cost = 1.0);
	~PathConnection() {}
	double GetCost() { return m_cost; }
	void SetCost(double cost) { m_cost = cost; }
	PathNode* GetFromNode() { return m_pFromNode; }
	PathNode* GetToNode() { return m_pToNode; }
	// For Lab 8.
	void SetFromNode(PathNode* node) { m_pFromNode = node; }
	void SetToNode(PathNode* node) { m_pToNode = node; }
private:
	// The cost of a PathConnection is the movement cost to go from one node (from) to another (to). This is the G cost.
	double m_cost; // G cost.
	PathNode *m_pFromNode, *m_pToNode;
};

struct NodeRecord // Used by A* to keep track of journey from the start.
{ // Everything public by default.
	NodeRecord(PathNode* n = nullptr) :m_node(n), m_connection(nullptr), m_fromRecord(nullptr), m_costSoFar(0.0), m_totalCost(0.0){	}
	PathNode* m_node;
	PathConnection* m_connection;
	NodeRecord* m_fromRecord;
	double m_costSoFar;
	double m_totalCost;
};
#endif
