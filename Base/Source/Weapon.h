#ifndef WEAPON_H
#define WEAPON_H

#include "ObjectClass.h"

class CWeapon : public CObjectClass
{
public:
	CWeapon(void);
	CWeapon(CWeapon &Copy);
	~CWeapon(void);

	void SetFirerate(float Firerate);
	void SetReloadspeed(float Reloadspeed);
	void SetcurrentBullets(int currentBullets);
	void SetnumBullets(int numBullets);
	void SetnumBulletsTotal(int numBulletsTotal);
	void SetmaxBullets(int maxBullets);

	float GetFirerate();
	float GetReloadspeed();
	int GetcurrentBullets();
	int GetnumBullets();
	int GetnumBulletsTotal();
	int GetmaxBullets();

	void Reload();
	void Fire();

	void Init(float Firerate, float Reloadspeed, int currentBullets, int numBullets, int numBulletsTotal, int maxBullets, Vector3 Pos, Vector3 Scale, Mtx44 Rotate, int ID);

private:
	float m_Firerate, m_Reloadspeed;
	int m_currentBullets, m_numBullets, m_numBulletsTotal, m_maxBullets;
};

#endif
