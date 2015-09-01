#include "Skill.h"


Skill::Skill(void)
{
}


Skill::~Skill(void)
{
}

void Skill::Init(float lifeTime, float damage, float coolDown, float speed, bool ranged, Tag::ENTITIES_TAGS entity)
{
	this->m_lifeTime = lifeTime;
	this->m_damage = damage;
	this->m_coolDown = coolDown;
	this->m_ranged = ranged;
	this->m_lastUse = 0;
	this->m_speed = speed;
	this->m_tag.SetEntity(entity);
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

void Skill::SetCoolDown(float newCoolDown)
{
	this->m_coolDown = newCoolDown;
}

float Skill::GetLifeTime()
{
	return m_lifeTime;
}
	
float Skill::GetDamage()
{
	return m_damage;
}

float Skill::GetCoolDown()
{
	return m_coolDown;
}

float Skill::GetSpeed()
{
	return m_speed;
}

bool Skill::GetRanged()
{
	return m_ranged;
}

Tag Skill::GetTag()
{
	return m_tag;
}

void Skill::UpgradeDamage(float damageToIncrease)
{
	this->m_damage += damageToIncrease;
}