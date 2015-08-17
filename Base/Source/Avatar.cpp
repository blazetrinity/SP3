#include "Avatar.h"

Avatar::Avatar() : x(0), y(0), render(false), mesh(NULL)
{
}
	
Avatar::~Avatar()
{

}

Avatar::Avatar(Avatar &copy)
{
	this->setPostion(copy.getPosition_x(), copy.getPosition_y());
	this->setRender(copy.getRender());
	this->setMesh(copy.getMesh());
}

void Avatar::setMesh(Mesh* newmesh)
{
	if(newmesh != NULL)
	{
		this->mesh = newmesh;
	}
}
	
void Avatar::setPostion(float x, float y)
{
	this->x = x;
	this->y = y;
}
	
void Avatar::setRender(bool render)
{
	this->render = render;
}

Mesh* Avatar::getMesh()
{
	return mesh;
}
	
float Avatar::getPosition_x()
{
	return x;
}

float Avatar::getPosition_y()
{
	return y;
}

bool Avatar::getRender()
{
	return render;
}