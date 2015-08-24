#ifndef POSITIONNODE_H
#define POSITIONNODE_H

#include "Vector2.h"

class PositionNode
{
public:
	PositionNode(void);
	PositionNode(bool walkable, Vector2 worldPosition);
	~PositionNode(void);

	void Init(Vector2 worldPosition, bool walkable, int gridX, int gridY);
	void SetWalkable(bool walkable);
	void SetgCost(int g);
	void SethCost(int h);
	void SetParentNode(PositionNode* parentNode);
	int GetGridX();
	int GetGridY();
	int GetgCost();
	int GethCost();
	int GetfCost();
	bool GetWalkable();
	PositionNode GetParentNode();
	Vector2 GetWorldPosition();

	bool operator==( const PositionNode& Node ) const; //Equality check

protected:
	bool m_walkable;
	Vector2 m_worldPosition;
	int m_gCost;
	int m_hCost;

	int m_gridX;
	int m_gridY;

	PositionNode *m_parentNode;
};

#endif