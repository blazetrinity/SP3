#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Mtx44.h"
#include <vector>

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Mtx44 rotation;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);

	virtual void MoveForward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize) = 0;
	virtual void MoveBackward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize) = 0;
	virtual void MoveLeft(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize) = 0;
	virtual void MoveRight(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize) = 0;

};

#endif