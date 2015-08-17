#include "ObjectClass.h"

CObjectClass::CObjectClass()
{
	Rotate.SetToIdentity();
	Translate.SetZero();
	Scale.SetZero();
}

CObjectClass::CObjectClass(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 size, int ID)
{
	this->setTranslate(Translate);
	this->setScale(Scale);
	this->setRotate(Rotate);
	this->setID(ID);
	this->setBound(size);
}

CObjectClass::CObjectClass(CObjectClass &copy)
{
	this->setTranslate(copy.getTranslate());
	this->setScale(copy.getScale());
	this->setRotate(copy.getRotate());
	this->setID(copy.getID());
}

CObjectClass::~CObjectClass()
{

}

void CObjectClass::Init(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 size, int ID)
{
	this->setTranslate(Translate);
	this->setScale(Scale);
	this->setRotate(Rotate);
	this->setID(ID);
	this->setBound(size);
}

Mtx44 CObjectClass::getRotate()
{
	return Rotate;
}

Vector3 CObjectClass::getTranslate()
{
	return Translate;
}

Vector3 CObjectClass::getScale()
{
	return Scale;
}

Vector3 CObjectClass::getBoundMax()
{
	return BoundCheck.getBoundMax();
}

Vector3 CObjectClass::getBoundMin()
{
	return BoundCheck.getBoundMin();
}

int CObjectClass::getID()
{
	return ID;
}

void CObjectClass::setRotate(Mtx44 R)
{
	Rotate = R;
}

void CObjectClass::setTranslate(Vector3 T)
{
	Translate = T;
}

void CObjectClass::setScale(Vector3 S)
{
	Scale = S;
}

void CObjectClass::setBound(Vector3 size)
{
	Vector3 Min, Max;
	Min.Set((Translate.x - (size.x * Scale.x)/2),(Translate.y - (size.y * Scale.y)),(Translate.z - (size.z * Scale.z)/2));
	Max.Set((Translate.x + (size.x * Scale.x)/2),(Translate.y + (size.y * Scale.y)),(Translate.z + (size.z * Scale.z)/2));
	BoundCheck.setBound(Min,Max);
}

void CObjectClass::setID(int ID)
{
	this->ID = ID;
}