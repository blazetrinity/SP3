#include "CollisionIn2D.h"


CollisionIn2D::CollisionIn2D(void)
{
	m_minBound.Set(0,0);
	m_maxBound.Set(0,0);
}


CollisionIn2D::~CollisionIn2D(void)
{
}

void CollisionIn2D::Init(Vector2 position, Vector2 scale, float tileSize)
{
	this->m_tileSize = tileSize;
	CalcBound(position, scale);
}

void CollisionIn2D::CalcBound(Vector2 position, Vector2 scale)
{
	m_minBound.Set(position.x, position.y);
	m_maxBound.Set(position.x + m_tileSize * scale.x, position.y + m_tileSize * scale.y);
}

void CollisionIn2D::Reset()
{
	m_minBound.Set(0,0);
	m_maxBound.Set(0,0);
}

	
Vector2 CollisionIn2D::GetMinBound()
{
	return m_minBound;
}
	
Vector2 CollisionIn2D::GetMaxBound()
{
	return m_maxBound;
}

	
bool CollisionIn2D::CollideWith(CollisionIn2D* other)
{
	if(GetMinBound().x < other->GetMaxBound().x && GetMaxBound().x > other->GetMinBound().x && GetMinBound().y < other->GetMaxBound().y && GetMaxBound().y > other->GetMinBound().y) // Collide
	{
		return true;
	}

	return false;
}