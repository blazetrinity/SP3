#include "CharacterIn3D.h"


CCharacterIn3D::CCharacterIn3D(void)
{
}


CCharacterIn3D::~CCharacterIn3D(void)
{
}

void CCharacterIn3D::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->setTranslate(pos);
	this->setScale(Vector3(1,1,1));
	Mtx44 R;
	R.SetToIdentity();
	this->setRotate(R);
	this->m_CurrentWeapon = NULL;
	this->health = 100;

	m_camera.Init(pos, target, up);
}

void CCharacterIn3D::AddWeapon(CWeapon &newWeapon)
{
	CWeapon *temp = new CWeapon(newWeapon);

	this->WeaponList.push_back(temp);
	if(this->m_CurrentWeapon == NULL)
	{
		this->m_CurrentWeapon = WeaponList[0];
	}
}

CWeapon* CCharacterIn3D::GetCurrentWeapon()
{
	return m_CurrentWeapon;
}

void CCharacterIn3D::ChangeWeapon(int i)
{
	m_CurrentWeapon = WeaponList[i];
}

int CCharacterIn3D::GetHealth()
{
	return health;
}

void CCharacterIn3D::SetHealth(int newHealth)
{
	this->health = newHealth;
}

void CCharacterIn3D::ReduceHealth()
{
	--health;
}

Camera3 CCharacterIn3D::GetCamera()
{
	return m_camera;
}
	
void CCharacterIn3D::UpdateCamera(double dt,std::vector<unsigned char> &heightMap, Vector3 terrainsize)
{
	m_camera.Update(dt, heightMap, terrainsize);
}

void CCharacterIn3D::UpdateCameraStatus(const unsigned char key)
{
	m_camera.UpdateStatus(key);
}