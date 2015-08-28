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

	void SetCoolDown(float newCoolDown);

	float GetLifeTime();
	float GetDamage();
	float GetCoolDown();
	bool GetRanged();
	Tag GetTag();

	void UpgradeDamage(float damageToIncrease);

protected:
	float m_lifeTime;
	float m_damage;
	float m_coolDown;
	float m_lastUse;
	bool m_ranged;
	Tag m_tag;
};

#endif