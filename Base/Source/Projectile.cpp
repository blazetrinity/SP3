#include "Projectile.h"

static const float PROJECTILE_SPEED = 100.f;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

void Projectile::Init(Skill* skill, Vector2 direction)
{
	this->m_lifeTime = skill->GetLifeTime();
	this->m_damage = skill->GetDamage();
	this->m_tag = skill->GetTag();
	this->m_velocity = (direction * PROJECTILE_SPEED);
}
	
void Projectile::Update(CMap map, double dt)
{
	m_position = m_position + (m_velocity * dt);
}

void Projectile::SetVelocity(Vector2 velocity)
{
	this->m_velocity = velocity;
}

void Projectile::SetActive(bool active)
{
	this->m_active = active;
}

bool Projectile::GetActive()
{
	return m_active;
}