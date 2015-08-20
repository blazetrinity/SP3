#include "Projectile.h"

static const float PROJECTILE_SPEED = 100.f;

Projectile::Projectile(void)
{
	this->m_active = false;
}


Projectile::~Projectile(void)
{
}

void Projectile::Init(Skill* skill,Vector2 position, Vector2 direction, Mesh* mesh, int level)
{
	this->m_lifeTime = skill->GetLifeTime();
	this->m_damage = skill->GetDamage();
	this->m_tag = skill->GetTag();
	this->m_position = position;
	this->m_mesh = mesh;
	this->m_level = level;
	this->m_velocity = (direction * PROJECTILE_SPEED);
}
	
void Projectile::Update(CMap* map, double dt)
{
	m_position = m_position + (m_velocity * dt);
	m_lifeTime -= dt;
	this->CalcBound(m_velocity, m_scale);

	if(m_lifeTime <= 0)
	{
		m_active = false;
	}
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

Mesh* Projectile::GetMesh()
{
	return m_mesh;
}

int Projectile::GetLevel()
{
	return m_level;
}