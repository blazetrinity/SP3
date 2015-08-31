#include "Projectile.h"

static const float PROJECTILE_SPEED = 100.f;

Projectile::Projectile(void)
{
	this->m_active = false;
	this->m_sprite = NULL;
}

Projectile::~Projectile(void)
{
}

void Projectile::Init(Skill* skill,Vector2 position, Vector2 scale, Vector2 direction, SpriteAnimation* newSprite, int level, float tileSize)
{
	this->m_lifeTime = skill->GetLifeTime();
	this->m_damage = skill->GetDamage();
	this->m_tag = skill->GetTag();

	this->m_sprite = newSprite;
	
	this->m_sprite->m_anim = newSprite->m_anim;
	this->m_sprite->Reset();

	this->m_level = level;
	this->m_velocity = (direction * PROJECTILE_SPEED);
	GameObjectIn2D::Init(position, scale, tileSize);
}
	
void Projectile::Update(CMap* map, double dt)
{
	// New position next frame
	Vector2 tilePos( (map->GetmapOffset().x + (this->GetPosition().x - (this->m_velocity.x * dt))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil((float)(this->GetPosition().y + map->GetTileSize()) / map->GetTileSize())) );

	if(m_velocity.x > 0)
	{
		if(map->theScreenMap[tilePos.y][tilePos.x+1] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x+1] > 0)
		{
			
			m_position = m_position + (m_velocity * dt);
		}
		else
		{
			m_active = false;
		}
	}

	else if(m_velocity.x < 0)
	{
		if(map->theScreenMap[tilePos.y][tilePos.x] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x] > 0)
		{
			
			m_position = m_position + (m_velocity * dt);
		}
		else
		{
			m_active = false;
		}
	}

	if(m_velocity.y > 0)
	{
		if(map->theScreenMap[tilePos.y][tilePos.x] > 0 && map->theScreenMap[tilePos.y][tilePos.x+1] > 0)
		{
			
			m_position = m_position + (m_velocity * dt);
		}
		else
		{
			m_active = false;
		}
	}

	else if(m_velocity.y < 0)
	{
		if(map->theScreenMap[tilePos.y+1][tilePos.x] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x+1] > 0)
		{
			
			m_position = m_position + (m_velocity * dt);
		}
		else
		{
			m_active = false;
		}
	}

	m_lifeTime -= dt;

	this->CalcBound(m_position, m_scale);
	this->m_sprite->Update(dt);

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

float Projectile::GetDamage()
{
	return m_damage;
}

bool Projectile::GetActive()
{
	return m_active;
}

SpriteAnimation* Projectile::GetMesh()
{
	return m_sprite;
}

int Projectile::GetLevel()
{
	return m_level;
}

Tag Projectile::GetTag()
{
	return m_tag;
}