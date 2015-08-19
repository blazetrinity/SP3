#ifndef SKILL_H
#define SKILL_H

class Skill
{
public:
	Skill(void);
	~Skill(void);

	virtual void Init(float lifeTime, float damage, float coolDown, bool ranged);
	virtual void Update(double dt);
	virtual bool Use();

	float GetLifeTime();
	float GetDamage();
	bool GetRanged();

protected:
	float m_lifeTime;
	float m_damage;
	float m_coolDown;
	float m_lastUse;
	bool m_ranged;
};

#endif