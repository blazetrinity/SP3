#include "Strategy_Chase.h"
static const float UPDATEPATHMAXTIMER = 5.f;

Strategy_Chase::Strategy_Chase(void)
{
}


Strategy_Chase::~Strategy_Chase(void)
{
}

void Strategy_Chase::Init(AstarPathfind* path)
{
	this->m_path = path;
	this->m_updatePathTimer = UPDATEPATHMAXTIMER;
	bool m_updatePath = false;
	this->m_nodeIndex = 0;
}

void Strategy_Chase::Update(Vector2* playerPos, Vector2* enemyPos, Vector2* enemyVel, double* dt)
{
	m_updatePathTimer -= *dt;

	// Check when to update path
	if(m_updatePathTimer <= 0)
	{
		m_updatePath = true;
		m_updatePathTimer = UPDATEPATHMAXTIMER;
	}

	if(m_updatePath)
	{
		this->m_pathFound = m_path->FindPath(*enemyPos, *playerPos);
		this->m_nodeIndex = 0;
		m_currentNode = m_pathFound[m_nodeIndex];
	}

	// Check when to update index
	float dist = (m_currentNode.GetWorldPosition() - *enemyPos).Length();

	// Update Index
	if(dist <= 0.5f)
	{
		m_nodeIndex++;
		m_currentNode = m_pathFound[m_nodeIndex];
	}

	Vector2 Dir = (m_currentNode.GetWorldPosition() - *enemyPos).Normalized();
	*enemyVel = Dir * 100;
}