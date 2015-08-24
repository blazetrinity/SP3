#ifndef STRATEGY_H
#define STRATEGY_H

#include "Vector2.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);
	virtual void Update();
};

#endif