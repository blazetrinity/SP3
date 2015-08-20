#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjectIn2D.h"
#include "Skill.h"
#include "Map.h"
#include "Tag.h"

class Projectile : public GameObjectIn2D
{
public:
	Projectile(void);
	~Projectile(void);

	virtual void Init(Skill* skill, Vector2 direction);
	virtual void Update(CMap map, double dt);
	void SetVelocity(Vector2 velocity);
	void SetActive(bool active);
	bool GetActive();
protected:
	float m_lifeTime;
	float m_damage;
	Vector2 m_velocity;
	Tag m_tag;
	bool m_active;
};

#endif