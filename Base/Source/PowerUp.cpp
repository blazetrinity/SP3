#include "PowerUp.h"


PowerUp::PowerUp(void)
{
}


PowerUp::~PowerUp(void)
{
}

void PowerUp::Init(Vector2 position, Vector2 scale, float tileSize, float lifeTime, float modifiedValue, ITEM_TYPE newType, Mesh* newMesh, int level)
{
	this->m_lifeTime = this->m_maxLifeTime = lifeTime;
	this->m_modifiedValue = modifiedValue;
	Item::Init(position, scale, tileSize, newType, newMesh, level);
}

void PowerUp::Update(double dt)
{
	this->m_lifeTime -= dt;
}

void PowerUp::StoreOriginalValue(float attribute)
{
	this->m_originalValue = attribute;
}
	
float PowerUp::GetOriginalValue()
{
	return m_originalValue;
}
	
float PowerUp::GetmodifiedValue()
{
	return m_modifiedValue;
}

void PowerUp::ResetLifeTime()
{
	this->m_lifeTime = this->m_maxLifeTime;
}

float PowerUp::GetLifeTime()
{
	return m_lifeTime;
}