#ifndef GAMEOBJECTIN2D_H
#define GAMEOBJECTIN2D_H

#include "collisionin2d.h"

class GameObjectIn2D : public CollisionIn2D
{
public:
	GameObjectIn2D(void);
	~GameObjectIn2D(void);

	virtual void Init(Vector2 position, Vector2 scale, float mass, float tileSize);

	void SetPosition(Vector2 position);

	Vector2 GetPosition();
	Vector2 GetScale();

protected:
	Vector2 m_position;
	Vector2 m_scale;
	float m_mass;
};

#endif