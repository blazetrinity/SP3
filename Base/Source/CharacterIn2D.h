#ifndef CHARACTERIN2D_H
#define CHARACTERIN2D_H

#include "GameObjectIn2D.h"
#include "Map.h"

class CharacterIn2D : public GameObjectIn2D
{
public:
	CharacterIn2D(void);
	~CharacterIn2D(void);

	virtual void Init(Vector2 position, Vector2 scale, float mass, float tileSize);
	virtual void Update(CMap *map, double dt, bool topDown);
	void UpdateTopDown(CMap *map, double dt);
	void UpdateSideView(CMap *map, double dt);

protected:
	Vector2 m_velocity;
	Vector2 m_facingNormal;
};

#endif