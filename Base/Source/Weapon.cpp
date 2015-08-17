#include "Weapon.h"

CWeapon::CWeapon(void)
{
}

CWeapon::CWeapon(CWeapon &Copy)
{
	this->SetFirerate(Copy.GetFirerate());
	this->SetReloadspeed(Copy.GetReloadspeed());
	this->SetcurrentBullets(Copy.GetcurrentBullets());
	this->SetnumBullets(Copy.GetnumBullets());
	this->SetnumBulletsTotal(Copy.GetnumBulletsTotal());
	this->SetmaxBullets(Copy.GetmaxBullets());
	this->setTranslate(Copy.getTranslate());
	this->setScale(Copy.getScale());
	this->setRotate(Copy.getRotate());
	this->setID(Copy.getID());
}

CWeapon::~CWeapon(void)
{
}

void CWeapon::Init(float Firerate, float Reloadspeed, int currentBullets, int numBullets, int numBulletsTotal, int maxBullets, Vector3 Pos, Vector3 Scale, Mtx44 Rotate, int ID)
{
	this->SetFirerate(Firerate);
	this->SetReloadspeed(Reloadspeed);
	this->SetcurrentBullets(currentBullets);
	this->SetnumBullets(numBullets);
	this->SetnumBulletsTotal(numBulletsTotal);
	this->SetmaxBullets(maxBullets);
	this->setTranslate(Pos);
	this->setScale(Scale);
	this->setRotate(Rotate);
	this->setID(ID);
}

void CWeapon::SetFirerate(float Firerate)
{
	this->m_Firerate = Firerate;
}
	
void CWeapon::SetReloadspeed(float Reloadspeed)
{
	this->m_Reloadspeed = Reloadspeed;
}

void CWeapon::SetcurrentBullets(int currentBullets)
{
	this->m_currentBullets = currentBullets;
}

void CWeapon::SetnumBullets(int numBullets)
{
	this->m_numBullets = numBullets;
}
	
void CWeapon::SetnumBulletsTotal(int numBulletsTotal)
{
	this->m_numBulletsTotal = numBulletsTotal;
}

void CWeapon::SetmaxBullets(int maxBullets)
{
	this->m_maxBullets = maxBullets;
}

float CWeapon::GetFirerate()
{
	return m_Firerate;
}
	
float CWeapon::GetReloadspeed()
{
	return m_Reloadspeed;
}

int CWeapon::GetcurrentBullets()
{
	return m_currentBullets;
}

int CWeapon::GetnumBullets()
{
	return m_numBullets;
}
	
int CWeapon::GetnumBulletsTotal()
{
	return m_numBulletsTotal;
}
int CWeapon::GetmaxBullets()
{
	return m_maxBullets;
}
	
void CWeapon::Reload()
{
	if(m_numBulletsTotal > 0)
	{
		if(m_currentBullets == 0)
		{
			if((m_numBulletsTotal - m_numBullets) >= 0)
			{
				if(this->getID() == CObjectClass::GEO_AK)
				{
					m_numBulletsTotal -= m_numBullets;
				}
				m_currentBullets = m_numBullets;
			}

			else
			{
				m_currentBullets += m_numBulletsTotal;
				m_numBulletsTotal = 0;
			}
		}

		else if(m_currentBullets > 0)
		{
			if((m_numBulletsTotal - (m_numBullets - m_currentBullets)) >= 0)
			{
				if(this->getID() == CObjectClass::GEO_AK)
				{
					m_numBulletsTotal -= (m_numBullets - m_currentBullets);
				}
				m_currentBullets = m_numBullets;
			}

			else
			{
				m_currentBullets += m_numBulletsTotal;
				m_numBulletsTotal = 0;
			}
		}
	}
}

void CWeapon::Fire()
{
	--m_currentBullets;
}