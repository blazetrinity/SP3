#ifndef COLLISIONIN2D_H
#define COLLISIONIN2D_H

#include "Vector2.h"

class CollisionIn2D
{
public:
	CollisionIn2D();
	virtual ~CollisionIn2D();

	virtual void Init(Vector2 position, Vector2 scale, float tileSize);
	virtual void CalcBound(Vector2 position, Vector2 scale);
	virtual void Reset();

	Vector2 GetMinBound();
	Vector2 GetMaxBound();

	bool CollideWith(CollisionIn2D* other);

protected:
	Vector2 m_minBound, m_maxBound;
	float m_tileSize;
};

#endif