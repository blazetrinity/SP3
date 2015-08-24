#pragma once
#include "Strategy.h"
#include "Map.h"

class Strategy_Patrol : public Strategy
{
public:
	Strategy_Patrol(void);
	~Strategy_Patrol(void);

	virtual void Update(CMap *m_cMap, Vector2* Position, Vector2* Velocity, Vector2* Direction, double* dt);

private:
	
	static const float m_CHANGEFACINGTIMER;
	float m_changeFacingTimer;
	bool m_changeFacing;
};
