#pragma once
#ifndef _PATHMANAGER_H_
#define _PATHMANAGER_H_
#include "Pathing.h"
#include <vector>

class PathManager
{
public:
	static void GetShortestPath(PathNode* start, PathNode* goal); // A*!!!
	static NodeRecord* GetSmallestNode();
	static std::vector<NodeRecord*>& OpenList();
	static std::vector<NodeRecord*>& ClosedList();
	static bool ContainsNode(std::vector<NodeRecord*>& list, PathNode* n);
	static NodeRecord* GetNodeRecord(std::vector<NodeRecord*>& list, PathNode* n);
	static double HEuclid(const PathNode* start, const PathNode* goal);
	static double HManhat(const PathNode* start, const PathNode* goal);
	static void DrawPath();
	static void DrawLists();
	static void ClearPath();
	static void ClearLists();
	static void DrawSmoothPath(); // Draw utility.
	static void SmoothPath(); // Smoothing algorithm.
	static bool CheckPathSegment(PathConnection* c); // LOS utility.
	static std::vector<PathConnection*>& GetPath();
private:
	PathManager() {}
private:
	static std::vector<NodeRecord*> s_open; // Used in A* algorithm.
	static std::vector<NodeRecord*> s_closed; // Used in A* algorithm.

	static std::vector<PathConnection*> s_path; // Will be used by agent to move.
	static std::vector<PathConnection*> s_pathSmooth; // For Lab 8.
};

typedef PathManager PAMA;

#endif
