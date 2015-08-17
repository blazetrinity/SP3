#pragma once
#include "Mesh.h"

class Avatar
{
private:
	float x;
	float y;
	bool render;
	Mesh* mesh;

public:
	Avatar();
	~Avatar();
	Avatar(Avatar &copy);

	void setMesh(Mesh* mesh);
	void setPostion(float x, float y);
	void setRender(bool render);

	Mesh* getMesh();
	float getPosition_x();
	float getPosition_y();
	bool getRender();
};

