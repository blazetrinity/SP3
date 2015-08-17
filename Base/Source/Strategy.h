#ifndef STRATEGY_H
#define STRATEGY_H

#include "Vector2.h"
#include "Map.h"

class Strategy
{
public:
	Strategy(void);
	~Strategy(void);
	virtual void Update(CMap*, Vector2*, Vector2*, Vector2*, double*)=0;
};

#endif