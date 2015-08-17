#include "BulletInfo.h"


CBulletInfo::CBulletInfo(void): m_bStatus(false)
{
}


CBulletInfo::~CBulletInfo(void)
{
}

void CBulletInfo::Init(const Vector3 position, const Vector3 direction, const Vector3 scale, const Mtx44 rotation,const Vector3 size, const int ID, const float speed, const float lifetime)
{
	this->SetDirection(direction);
	this->SetSpeed(speed);
	this->SetLifetime(lifetime);
	this->SetStatus(true);
	this->CObjectClass::Init(position,scale,rotation,size,ID);
}


void CBulletInfo::SetStatus(const bool bStatus)
{
	this->m_bStatus = bStatus;
}
	
bool CBulletInfo::GetStatus()
{
	return m_bStatus;
}
	
void CBulletInfo::SetDirection(const Vector3 direction)
{
	this->direction = direction;
}
	
Vector3 CBulletInfo::GetDirection()
{
	return direction;
}
	
void CBulletInfo::SetSpeed(const float speed)
{
	this->speed = speed;
}

float CBulletInfo::GetSpeed()
{
	return speed;
}

void CBulletInfo::SetLifetime(const float lifetime)
{
	this->lifetime = lifetime;
}

float CBulletInfo::GetLifetime()
{
	return lifetime;
}

void CBulletInfo::Update(const double dt)
{
	//Check if the bullet is active
	if(GetStatus() == true)
	{
		//Update the position of the bullet
		setTranslate(getTranslate() + GetDirection() * GetSpeed() * dt);

		this->setBound(Vector3(2,1,2));

		//Update the lifetime
		SetLifetime(GetLifetime() - dt);

		//Checkif the lifetime is gone
		if(GetLifetime() < 0)
		{
			SetStatus(false);
		}
	}
}