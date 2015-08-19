#include "Skill.h"


Skill::Skill(void)
{
}


Skill::~Skill(void)
{
}

void Skill::Init(float lifeTime, float damage, float coolDown, bool ranged)
{
	this->m_lifeTime = lifeTime;
	this->m_damage = damage;
	this->m_coolDown = coolDown;
	this->m_ranged = ranged;
	this->m_lastUse = 0;
}
	
void Skill::Update(double dt)
{
	this->m_lastUse += dt;
}

bool Skill::Use()
{
	if(m_lastUse > m_coolDown)
	{
		m_lastUse = 0;
		return true;
	}

	return false;
}

float Skill::GetLifeTime()
{
	return m_lifeTime;
}
	
float Skill::GetDamage()
{
	return m_damage;
}
	
bool Skill::GetRanged()
{
	return m_ranged;
}