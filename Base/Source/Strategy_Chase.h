#ifndef STRATEGY_CHASE_H
#define STRATEGY_CHASE_H

#include "Strategy.h"
#include "AstarPathfind.h"

class Strategy_Chase :public Strategy
{
public:
	Strategy_Chase(void);
	~Strategy_Chase(void);
	virtual void Init(AstarPathfind* path);
	virtual void Update(Vector2* playerPos, Vector2* enemyPos, Vector2* enemyVel, double* dt);

protected:
	AstarPathfind* m_path;
	vector<PositionNode> m_pathFound;
	PositionNode m_currentNode;
	float m_updatePathTimer;
	bool m_updatePath;
	int m_nodeIndex;
};

#endif