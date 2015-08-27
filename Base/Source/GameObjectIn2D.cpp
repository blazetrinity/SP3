#include "GameObjectIn2D.h"

GameObjectIn2D::GameObjectIn2D(void)
{
	this->m_position.Set(0,0);
	this->m_scale.Set(32,32);
}

GameObjectIn2D::~GameObjectIn2D(void)
{
}

void GameObjectIn2D::Init(Vector2 position, Vector2 scale, float tileSize)
{
	this->m_position = position;
	this->m_scale = scale;
	CollisionIn2D::Init(m_position, m_scale, tileSize);
}

void GameObjectIn2D::SetPosition(Vector2 position)
{
	this->m_position = position;
	this->CalcBound(m_position, m_scale);
}

Vector2 GameObjectIn2D::GetPosition()
{
	return m_position;
}
	
Vector2 GameObjectIn2D::GetScale()
{
	return m_scale;
}