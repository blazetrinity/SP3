/******************************************************************************/
/*!
\file	Bound.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Bound
*/
/******************************************************************************/

#include "Bound.h"

/******************************************************************************/
/*!
\brief
CBound default constructor
*/
/******************************************************************************/
CBound::CBound(void)
{
}

/******************************************************************************/
/*!
\brief
CBound deconstructor
*/
/******************************************************************************/
CBound::~CBound(void)
{
}

/******************************************************************************/
/*!
\brief
Set the Bound

\param min
	Sets the Minimum Bound

\param Max
	Sets the Maximum Bound
*/
/******************************************************************************/
void CBound::setBound(Vector3 Min, Vector3 Max){
	MinBound = Min;
	MaxBound = Max;
}

/******************************************************************************/
/*!
\brief
Get the Minimum Bound

\return
	Returns the Minimum Bound
*/
/******************************************************************************/
Vector3 CBound::getBoundMin(void){
	return MinBound;
}

/******************************************************************************/
/*!
\brief
Get the Maximum Bound

\param Gets the Maximum Bound
*/
/******************************************************************************/
Vector3 CBound::getBoundMax(void){
	return MaxBound;
}