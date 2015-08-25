#include "PositionNode.h"


PositionNode::PositionNode(void)
{
}

PositionNode::PositionNode(bool walkable, Vector2 worldPosition)
{
	this->m_walkable = walkable;
	this->m_worldPosition = worldPosition;
}

PositionNode::~PositionNode(void)
{
}

void PositionNode::Init(Vector2 worldPosition, bool walkable, int gridX, int gridY)
{
	this->m_worldPosition = worldPosition;
	this->m_walkable = walkable;
	this->m_gridX = gridX;
	this->m_gridY = gridY;
	this->m_gCost = 0;
	this->m_hCost = 0;
}

void PositionNode::SetWalkable(bool walkable)
{
	this->m_walkable = walkable;
}

void PositionNode::SetgCost(int g)
{
	this->m_gCost = g;
}
	
void PositionNode::SethCost(int h)
{
	this->m_hCost = h;
}

void PositionNode::SetParentNode(PositionNode* parentNode)
{
	this->m_parentNode = parentNode;
}

int PositionNode::GetGridX()
{
	return m_gridX;
}

int PositionNode::GetGridY()
{
	return m_gridY;
}

int PositionNode::GetgCost()
{
	return m_gCost;
}

int PositionNode::GethCost()
{
	return m_hCost;
}

int PositionNode::GetfCost()
{
	return m_gCost + m_hCost;
}

bool PositionNode::GetWalkable()
{
	return m_walkable;
}

bool PositionNode::operator==( const PositionNode& Node ) const
{
	return this->m_worldPosition == Node.m_worldPosition;
}

PositionNode* PositionNode::GetParentNode()
{
	return m_parentNode;
}

Vector2 PositionNode::GetWorldPosition()
{
	return m_worldPosition;
}