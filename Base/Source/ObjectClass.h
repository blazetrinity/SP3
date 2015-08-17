/******************************************************************************/
/*!
\file	ObjectClass.h
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define a Scene Object
*/
/******************************************************************************/

#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Bound.h"
#include "LoadHmap.h"

/******************************************************************************/
/*!
		Class CObjectClass:
\brief	Defines a Scene Object and its methods
*/
/******************************************************************************/
class CObjectClass
{
private:
	Mtx44 Rotate;
	Vector3 Translate;
	Vector3 Scale;
	CBound BoundCheck;
	int ID;

public:
	CObjectClass();
	CObjectClass(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 size, int ID); 
	CObjectClass(CObjectClass &copy);
	~CObjectClass();

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_QUAD2,
		GEO_TEXT,
		GEO_SKYPLANE,
		GEO_TERRAIN,
		GEO_POND,
		GEO_BULLET,
		GEO_OIL_DRUM,
		GEO_SHACK,
		GEO_BUILDING,
		GEO_SPRITE_ANIMATION,
		GEO_BOBA_TARGET,
		GEO_VADER_TARGET,
		GEO_AK,
		GEO_DEAGLE,
		GEO_HEALTH,
		GEO_ENEMY,
		GEO_RAIN,
		GEO_LIGHT_DEPTH_QUAD,

		NUM_GEOMETRY,
	};

	virtual void Init(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 size, int ID);

	Mtx44 getRotate(void);
	
	Vector3 getTranslate(void);
	Vector3 getScale(void);
	Vector3 getBoundMax(void);
	Vector3 getBoundMin(void);

	int getID();

	void setRotate(Mtx44 R);
	void setTranslate(Vector3 T);
	void setScale(Vector3 S);
	void setBound(Vector3 size);
	void setID(int ID);
};

#endif