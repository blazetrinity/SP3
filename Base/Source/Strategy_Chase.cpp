#include "Strategy_Chase.h"
static const float UPDATEPATHMAXTIMER = 2.5f;

Strategy_Chase::Strategy_Chase(void)
{
}


Strategy_Chase::~Strategy_Chase(void)
{
	if(m_path != NULL)
	{
		delete m_path;
		m_path = NULL;
	}
}

void Strategy_Chase::Init(AstarPathfind* path)
{
	this->m_path = path;
	this->m_updatePathTimer = 0;
	bool m_updatePath = false;
	this->m_nodeIndex = 0;
}

void Strategy_Chase::Update(Vector2* playerPos, Vector2* enemyPos, Vector2* enemyVel, double* dt)
{
	float distFromPlayer = (*enemyPos - *playerPos).Length();

	if(distFromPlayer > 16.f)
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
			if(m_pathFound.size() != 0)
			{
				m_currentNode = m_pathFound[m_nodeIndex];
			}
			else
			{
				m_currentNode = NULL;
			}
			m_updatePath = false;
		}

		if(m_currentNode)
		{
			// Check when to update index
			float dist = (m_currentNode->GetWorldPosition() - *enemyPos).Length();

			// Update Index
			if(dist <= 1.f)
			{
				*enemyPos = m_currentNode->GetWorldPosition();
				if(m_nodeIndex < m_pathFound.size()-1)
				{
					m_nodeIndex++;
					m_currentNode = m_pathFound[m_nodeIndex];
				}
			}

			Vector2 Dir;

			if((m_currentNode->GetWorldPosition() - *enemyPos).Length() != 0)
			{
				Dir = (m_currentNode->GetWorldPosition() - *enemyPos).Normalized();
			}
			else
			{
				Dir.Set(0, 0);
			}

			*enemyVel = Dir * 100;
		}
	}
}