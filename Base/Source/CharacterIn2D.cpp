#include "CharacterIn2D.h"

static const float GRAVITY = 9.8f;
static const float FRICTIONCOEFFICENT = 3.5f;
const float CharacterIn2D::VIEWOFFSET = 64.f;

CharacterIn2D::CharacterIn2D(void)
{
	m_skill = NULL;
}


CharacterIn2D::~CharacterIn2D(void)
{
}

void CharacterIn2D::Init(Vector2 position, Vector2 scale, float mass, float tileSize, Skill* skill, int health)
{
	GameObjectIn2D::Init(position, scale, tileSize);
	if(m_skill)
	{
		delete m_skill;
	}

	this->m_viewPosition = m_position + (m_facingNormal * VIEWOFFSET);
	this->m_skill = skill;
	this->m_health = this->m_maxHealth = health;
	this->m_mass = mass;
	this->m_velocity.Set(0, 0);
}

void CharacterIn2D::Update(CMap *map,double dt, bool topDown)
{
	if(m_velocity.Length() != 0)
	{
		m_facingNormal = m_velocity.Normalized();
	}

	if(topDown)
	{
		UpdateTopDown(map, dt);
	}
	else
	{
		UpdateSideView(map, dt);
	}
	this->CalcBound(m_position, m_scale);
}

void CharacterIn2D::UpdateTopDown(CMap *map, double dt)
{
	//Calculate the friction force
	static float frictionForce = this->m_mass * GRAVITY * FRICTIONCOEFFICENT;
	Vector2 frictionDirection;

	if(m_velocity.Length() > 0)
	{
		frictionDirection = -m_velocity.Normalized();
	}
	else
	{
		frictionDirection.Set(0,0);
	}

	Vector2 frictionInOppositeDirection = frictionDirection * frictionForce;

	if(frictionInOppositeDirection.Length() < 1.f)
	{
		frictionInOppositeDirection.Set(0,0);
	}

	m_velocity = m_velocity + (frictionInOppositeDirection * dt);

	if(m_velocity.Length() < 1.f)
	{
		m_velocity.Set(0,0);
	}

	// New position next frame
	Vector2 tilePos((map->GetmapOffset().x + (this->GetPosition().x + (this->m_velocity.x * dt))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil((float)((this->GetPosition().y + this->m_velocity.y * dt) + map->GetTileSize()) / map->GetTileSize())));
	Vector2 viewPos((map->GetmapOffset().x + (this->m_viewPosition.x + (this->m_velocity.x * dt))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil((float)((this->m_viewPosition.y + this->m_velocity.y * dt) + map->GetTileSize()) / map->GetTileSize())));

	if(m_facingNormal.x > 0)
	{
		if(map->theScreenMap[viewPos.y][viewPos.x+1] > 0 && map->theScreenMap[viewPos.y+1][viewPos.x+1] > 0)
		{
			this->m_viewPosition.x = this->m_viewPosition.x + (m_velocity.x * dt);
		}

		if(map->theScreenMap[tilePos.y][tilePos.x+1] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x+1] > 0)
		{
			this->m_position.x = this->m_position.x + (m_velocity.x * dt);
		}
		else
		{
			m_velocity.x = 0;
		}
	}

	else if(m_facingNormal.x < 0)
	{
		if(map->theScreenMap[viewPos.y][viewPos.x] > 0 && map->theScreenMap[viewPos.y+1][viewPos.x] > 0)
		{
			this->m_viewPosition.x = this->m_viewPosition.x + (m_velocity.x * dt);
		}

		if(map->theScreenMap[tilePos.y][tilePos.x] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x] > 0)
		{
			this->m_position.x = this->m_position.x + (m_velocity.x * dt);
		}
		else
		{
			m_velocity.x = 0;
		}
	}

	if(m_facingNormal.y > 0)
	{
		if(map->theScreenMap[viewPos.y][viewPos.x] > 0 && map->theScreenMap[viewPos.y][viewPos.x+1] > 0)
		{
			this->m_viewPosition.y = this->m_viewPosition.y + (m_velocity.y * dt);
		}

		if(map->theScreenMap[tilePos.y][tilePos.x] > 0 && map->theScreenMap[tilePos.y][tilePos.x+1] > 0)
		{
			this->m_position.y = this->m_position.y + (m_velocity.y * dt);
		}
		else
		{
			m_velocity.y = 0;
		}
	}

	else if(m_facingNormal.y < 0)
	{
		if(map->theScreenMap[viewPos.y+1][viewPos.x] > 0 && map->theScreenMap[viewPos.y+1][viewPos.x+1] > 0)
		{
			this->m_viewPosition.y = this->m_viewPosition.y + (m_velocity.y * dt);
		}

		if(map->theScreenMap[tilePos.y+1][tilePos.x] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x+1] > 0)
		{
			this->m_position.y = this->m_position.y + (m_velocity.y * dt);
		}
		else
		{
			m_velocity.y = 0;
		}
	}
}

void CharacterIn2D::UpdateSideView(CMap *map, double dt)
{

}

bool CharacterIn2D::Attack()
{
	return false;
}

Skill* CharacterIn2D::GetSkill()
{
	return m_skill;
}

Vector2 CharacterIn2D::GetFacingNormal()
{
	return m_facingNormal;
}

bool CharacterIn2D::TakeDamage(float damage)
{
	m_health -= damage;

	if(m_health <= 0)
	{
		return true;
	}

	return false;
}

void CharacterIn2D::SetHealth(int health)
{
	this->m_health = health;
}