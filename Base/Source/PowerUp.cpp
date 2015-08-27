#include "PowerUp.h"


PowerUp::PowerUp(void)
{
}


PowerUp::~PowerUp(void)
{
}

void PowerUp::Init(Vector2 position, Vector2 scale, float tileSize, float lifeTime, float modifiedValue, ITEM_TYPE newType, Mesh* newMesh)
{
	this->m_lifeTime = this->m_maxLifeTime = lifeTime;
	this->m_modifiedValue = modifiedValue;
	Item::Init(position, scale, tileSize, newType, newMesh);
}

void PowerUp::Update(double dt)
{
	this->m_lifeTime -= dt;
}

void PowerUp::AppliedPowerUp(float *attribute)
{
	this->m_originalValue = *attribute;
	*attribute = this->m_modifiedValue;
}
	
void PowerUp::RemovePowerUp(float *attribute)
{
	*attribute = this->m_originalValue;
}

void PowerUp::ResetLifeTime()
{
	this->m_lifeTime = this->m_maxLifeTime;
}