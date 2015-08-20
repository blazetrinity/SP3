#ifndef SKILL_H
#define SKILL_H

#include "Tag.h"

class Skill
{
public:
	Skill(void);
	~Skill(void);

	virtual void Init(float lifeTime, float damage, float coolDown, bool ranged, Tag::ENTITIES_TAGS entity);
	virtual void Update(double dt);
	virtual bool Use();

	float GetLifeTime();
	float GetDamage();
	bool GetRanged();
	Tag GetTag();

protected:
	float m_lifeTime;
	float m_damage;
	float m_coolDown;
	float m_lastUse;
	bool m_ranged;
	Tag m_tag;
};

#endif