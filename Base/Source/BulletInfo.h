#ifndef BULLETINFO_H
#define BULLETINFO_H

#include "ObjectClass.h"

class CBulletInfo : public CObjectClass
{
public:
	CBulletInfo(void);
	virtual ~CBulletInfo(void);

	void Init(const Vector3 position, const Vector3 direction, const Vector3 scale, const Mtx44 rotation,const Vector3 size, const int ID, const float speed, const float lifetime);
	
	void SetStatus(const bool bStatus);
	bool GetStatus();

	void SetDirection(const Vector3 direction);
	Vector3 GetDirection();
	
	void SetSpeed(const float speed);
	float GetSpeed();

	void SetLifetime(const float lifetime);
	float GetLifetime();

	void Update(const double dt);

protected:
	// Indicate the status of this bullet
	bool m_bStatus;
	Vector3 direction;
	float speed;
	float lifetime;
};

#endif

