#include "CharacterIn2D.h"

static const float GRAVITY = 9.8f;
static const float FRICTIONCOEFFICENT = 4.0f;

CharacterIn2D::CharacterIn2D(void)
{
}


CharacterIn2D::~CharacterIn2D(void)
{
}

void CharacterIn2D::Init(Vector2 position, Vector2 scale, float mass, float tileSize, Skill* skill)
{
	GameObjectIn2D::Init(position, scale, mass, tileSize);
	this->m_skill = skill;
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
	float frictionForce = this->m_mass * GRAVITY * FRICTIONCOEFFICENT;
	Vector2 frictionDirection;
	if(m_velocity.Length() != 0)
	{
		frictionDirection = -m_velocity.Normalized();
	}
	else
	{
		frictionDirection.Set(0,0);
	}

	Vector2 frictionInOppositeDirection = frictionDirection * frictionForce;

	m_velocity = m_velocity + (frictionInOppositeDirection * dt);

	// New position next frame
	Vector2 tilePos( (map->GetmapOffset().x + (this->GetPosition().x - (this->m_velocity.x * dt))) / map->GetTileSize() , map->GetNumOfTiles_Height() - (int) (ceil((float)(this->GetPosition().y + map->GetTileSize()) / map->GetTileSize())) );

	if(m_facingNormal.x > 0)
	{
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
		if(map->theScreenMap[tilePos.y+1][tilePos.x] > 0 && map->theScreenMap[tilePos.y+1][tilePos.x+1] > 0)
		{
			this->m_position.y = this->m_position.y + (m_velocity.y * dt);
		}
		else
		{
			m_velocity.y = 0;
		}
	}

	if(m_velocity.Length() < 1.f)
	{
		m_velocity.Set(0,0);
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