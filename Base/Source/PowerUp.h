#ifndef POWERUP_H
#define POWERUP_H

#include "item.h"
class PowerUp : public Item
{
public:
	PowerUp(void);
	~PowerUp(void);

	// Init the powerup
	virtual void Init(Vector2 position, Vector2 scale, float tileSize, float lifeTime, float modifiedvalue, ITEM_TYPE newType, Mesh* newMesh, int level);
	void Update(double dt);
	void StoreOriginalValue(float attribute);
	float GetOriginalValue();
	float GetmodifiedValue();
	void ResetLifeTime();
	float GetLifeTime();

protected:
	float m_lifeTime;
	float m_maxLifeTime;
	float m_modifiedValue;
	float m_originalValue;
};

#endif