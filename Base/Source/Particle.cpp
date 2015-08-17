#include "Particle.h"


CParticle::CParticle(void) : m_status(false), m_gravity(false)
{
}

CParticle::CParticle(Vector3 Pos, Vector3 Scale, Vector3 Vel, Mtx44 Rotate, Vector3 size, int ID, bool Status, bool Gravity)
{
	this->setTranslate(Pos);
	this->setScale(Scale);
	this->setvel(Vel);
	this->setRotate(Rotate);
	this->setBound(size);
	this->setID(ID);
	this->setstatus(Status);
	this->setgravity(Gravity);
}

CParticle::~CParticle(void)
{
}

Vector3 CParticle::getvel()
{
	return m_vel;
}

bool CParticle::getstatus()
{
	return m_status;
}

bool CParticle::getgravity()
{
	return m_gravity;
}

void CParticle::setvel(Vector3 newVel)
{
	this->m_vel = newVel;
}

void CParticle::setstatus(bool newstatus)
{
	this->m_status = newstatus;
}

void CParticle::setgravity(bool newgravity)
{
	this->m_gravity = newgravity;
}

void CParticle::Update(double dt)
{
	if(getID() == CObjectClass::GEO_RAIN)
	{
		static const Vector3 Gravity(0,-9.8,0);
		Vector3 Pos = getTranslate();
		m_vel += Gravity * dt;
		Pos += m_vel * dt;
		setTranslate(Pos);
	}
}