#ifndef CHARACTERIN2D_H
#define CHARACTERIN2D_H

#include "GameObjectIn2D.h"
#include "Map.h"
#include "Skill.h"

class CharacterIn2D : public GameObjectIn2D
{
public:
	CharacterIn2D(void);
	~CharacterIn2D(void);

	virtual void Init(Vector2 position, Vector2 scale, float mass, float tileSize, Skill* skill, int health);
	virtual void Update(CMap *map, double dt, bool topDown);
	void UpdateTopDown(CMap *map, double dt);
	void UpdateSideView(CMap *map, double dt);
	virtual bool Attack();
	virtual Skill* GetSkill();
	virtual Vector2 GetFacingNormal();
	virtual bool TakeDamage(float damage);
	virtual void SetHealth(int health);

protected:
	Vector2 m_velocity;
	Vector2 m_facingNormal;
	Skill* m_skill;
	int m_health;
	Vector2 m_viewPostion;
};

#endif