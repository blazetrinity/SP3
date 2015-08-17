/***********************************************************/
/*!
\file	Bound.h
\author	Malcolm Lim
\par	
\brief	Creates a bound and storing the Information
*/
/***********************************************************/

#ifndef BOUND_H
#define BOUND_H
#include "Vector3.h"

/***********************************************************/
/*!
	class CBound
\brief	Defines a bound and it's method
*/
/***********************************************************/
class CBound
{
public:
	CBound(void);
	~CBound(void);

	void setBound(Vector3 Min, Vector3 Max);

	Vector3 getBoundMin(void);
	Vector3 getBoundMax(void);

private:
	Vector3 MaxBound;
	Vector3 MinBound;
};

#endif