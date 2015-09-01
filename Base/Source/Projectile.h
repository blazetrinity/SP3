#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjectIn2D.h"
#include "Skill.h"
#include "Map.h"
#include "Tag.h"
#include "SpriteAnimation.h"

class Projectile : public GameObjectIn2D
{
public:
	Projectile(void);
	~Projectile(void);

	virtual void Init(Skill* skill,Vector2 position, Vector2 scale, Vector2 direction, SpriteAnimation* m_sprite, int level, float tileSize);
	virtual void Update(CMap* map, double dt);
	void SetVelocity(Vector2 velocity);
	void SetActive(bool active);
	float GetDamage();
	bool GetActive();
	SpriteAnimation* GetMesh();
	int GetLevel();
	Tag GetTag();

protected:
	float m_lifeTime;
	float m_damage;
	float m_speed;
	int m_level;
	Vector2 m_velocity;
	Tag m_tag;
	bool m_active;
	SpriteAnimation* m_sprite;
};

#endif