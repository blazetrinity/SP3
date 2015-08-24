#ifndef ASTARPATHFIND_H
#define ASTARPATHFIND_H
#include <iostream>
#include "PositionNode.h"
#include "Map.h"
#include <algorithm>

class AstarPathfind
{
public:
	AstarPathfind(void);
	~AstarPathfind(void);
	PositionNode NodeFromWorldPoint(Vector2 worldPosition);
	vector<PositionNode> FindPath(Vector2 startPos, Vector2 targetPos);
	vector<PositionNode> RetracePath(PositionNode startNode, PositionNode endNode);
	void GenerateGrid(CMap* map);
	vector<PositionNode> GetNeighbours(PositionNode node);
	int GetDistance(PositionNode nodeA, PositionNode nodeB);

protected:
	vector<vector<PositionNode>> grid;
	CMap *m_map;
};

#endif