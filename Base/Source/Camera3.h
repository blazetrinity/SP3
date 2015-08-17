#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "LoadHmap.h"

class Camera3 : public Camera
{
private:
	//For Jump use
	bool m_bJumping;
	Vector3 Gravity;
	Vector3 JumpVel;
	Vector3 JumpAccel;
	float JumpMaxSpeed;

public:
	enum PLAYER_STANCE
	{
		STANCE_STAND = 0,
		STANCE_CROUCH,
		STANCE_PRONE,
		STANCE_RUN,
		NUM_STANCE,
	};

	enum CAM_TYPE
	{
		LAND_CAM = 0,
		AIR_CAM,
		NUM_CAM_TYPE,
	};

	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	float totalpitch, totalyaw;

	CAM_TYPE sCameraType;

	bool myKeys[255];

	PLAYER_STANCE stance;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<unsigned char> &heightMap,Vector3 terrainSize);
	virtual void Reset();

	void UpdateStatus(const unsigned char key);

	virtual void SetCameraType(CAM_TYPE sCameraType);
	virtual CAM_TYPE GetCameraType();

	virtual void MoveForward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveBackward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveLeft(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveRight(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);

	virtual void TurnLeft(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void TurnRight(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void LookUp(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void LookDown(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void SpinClockWise(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void SpinCounterClockWise(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);

	virtual void Pitch(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Yaw(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Roll(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Walk(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Strafe(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Jump(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void UpdateJump(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void Run(bool run, std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	
	virtual void ChangeStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize, double dt);
	virtual void UpdateStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize, double dt);
};

#endif