#ifndef PARTICLE_H
#define PARTICLE_H

#include "ObjectClass.h"

class CParticle : public CObjectClass
{
private:
	Vector3 m_vel;
	bool m_status, m_gravity;

public:
	CParticle(void);
	CParticle(Vector3 Pos, Vector3 Scale, Vector3 Vel, Mtx44 Rotate, Vector3 size, int ID, bool Status, bool Gravity);
	~CParticle(void);

	Vector3 getvel();
	bool getstatus();
	bool getgravity();

	void setvel(Vector3 newVel);
	void setstatus(bool newstatus);
	void setgravity(bool newgravity);

	void Update(double dt);
};

#endif