#include "Strategy_Patrol.h"

const float Strategy_Patrol::m_CHANGEFACINGTIMER = 2.5f;

Strategy_Patrol::Strategy_Patrol(void) : m_changeFacing(false), m_changeFacingTimer(m_CHANGEFACINGTIMER)
{
}


Strategy_Patrol::~Strategy_Patrol(void)
{
}

void Strategy_Patrol::Update(CMap *m_cMap, Vector2* Position, Vector2* Velocity, Vector2* Direction, double* dt)
{
	m_changeFacingTimer -= *dt;

	if(m_changeFacingTimer <= 0)
	{
		m_changeFacing = true;
		m_changeFacingTimer = m_CHANGEFACINGTIMER;
	}

	if(m_changeFacing)
	{
		*Direction = -*Direction;
		m_changeFacing = false;
	}

	if(Direction->y > 0)
	{
		Velocity->y = 100;
	}
	else if(Direction->y < 0)
	{
		Velocity->y = -100;
	}
	else if(Direction->x > 0)
	{
		Velocity->x = 100;
	}
	else if(Direction->x < 0)
	{
		Velocity->x = -100;
	}
}