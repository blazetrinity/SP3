#include "EnemyIn3D.h"


CEnemyIn3D::CEnemyIn3D(void) : m_Speed(100), m_state(false)
{
}


CEnemyIn3D::~CEnemyIn3D(void)
{
}

void CEnemyIn3D::Update(Vector3 PlayerPos, double dt, Vector3 terrainSize, std::vector<unsigned char> *heightMap)
{
	vel = (PlayerPos - this->getTranslate()).Normalized();
	vel = m_Speed * vel;
	Vector3 newPos = this->getTranslate() + vel * dt;

	this->setTranslate(Vector3 (newPos.x, 10 + terrainSize.y * ReadHeightMap(*heightMap, newPos.x/terrainSize.x, newPos.z/terrainSize.z), newPos.z));
	this->setBound(Vector3(2,1,2));
}

void CEnemyIn3D::setState(bool newstate)
{
	this->m_state = newstate;
}

bool CEnemyIn3D::getState()
{
	return m_state;
}
