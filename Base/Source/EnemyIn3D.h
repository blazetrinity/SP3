#ifndef ENEMYINI3D_H
#define ENEMYINI3D_H

#include "ObjectClass.h"

class CEnemyIn3D : public CObjectClass
{
public:
	CEnemyIn3D(void);
	~CEnemyIn3D(void);

	void Update(Vector3 PlayerPos, double dt, Vector3 terrainSize, std::vector<unsigned char> *heightMap);
	bool getState();
	void setState(bool newstate);
	

private:
	Vector3 vel;
	float m_Speed;
	bool m_state;
};

#endif

