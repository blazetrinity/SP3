#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

static const float CAMERA_SPEED_WALK = 200.f;
static const float CAMERA_SPEED_RUN = 400.f;
static const float CAMERA_SPEED_CROUCH = 100.f;
static const float CAMERA_SPEED_PRONE = 50.f;
static float HEIGHT_OFFSET = 10;

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	stance = PLAYER_STANCE::STANCE_STAND;
	sCameraType = CAM_TYPE::LAND_CAM;
	JumpMaxSpeed = 200; 
	m_bJumping = false;
	Gravity.Set(0,-95,0);
	JumpVel.Set(0,0,0);
	JumpAccel.Set(0,50,0);
	totalpitch = 0;
	totalyaw = 0;

	for(int i = 0; i < 255; ++i)
	{
		myKeys[i] = false;
	}
}

void Camera3::Update(double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(myKeys['a'] == true)
	{
		Strafe(-dt,heightMap,terrainSize);
		myKeys['a'] = false;
	}
	if(myKeys['d'] == true)
	{
		Strafe(dt,heightMap,terrainSize);
		myKeys['d'] = false;
	}
	if(myKeys['w'] == true)
	{
		Walk(dt, heightMap, terrainSize);
		myKeys['w'] = false;
	}
	if(myKeys['s'] == true)
	{
		Walk(-dt, heightMap, terrainSize);
		myKeys['s'] = false;
	}
	if(myKeys['c'] == true)
	{
		ChangeStance(heightMap,terrainSize,dt);
		myKeys['c'] = false;
	}
	if(myKeys['z'] == true && m_bJumping == false)
	{
		Run(true, heightMap,terrainSize);
		myKeys['z'] = false;
	}
	if(myKeys['x'] == true && m_bJumping == false)
	{
		Run(false, heightMap,terrainSize);
		myKeys['x'] = false;
	}
	if(myKeys[32] == true)
	{
		Jump(dt, heightMap, terrainSize);
		myKeys[32] = false;
	}

	UpdateJump(dt, heightMap, terrainSize);

	if(!m_bJumping)
	{
		if(stance == PLAYER_STANCE::STANCE_STAND || stance == PLAYER_STANCE::STANCE_RUN)
		{
			if(sqrt((HEIGHT_OFFSET - 10) * (HEIGHT_OFFSET - 10)) < 0.1)
			{
				HEIGHT_OFFSET = 10;
			}
			else if(HEIGHT_OFFSET > 10)
			{
				HEIGHT_OFFSET -= 8 * dt;
			}
			else if(HEIGHT_OFFSET < 10)
			{
				HEIGHT_OFFSET += 8 * dt;
			}

			float yDiff = target.y - position.y;
			position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);	
			target.y = position.y + yDiff;
		}

		else if(stance == PLAYER_STANCE::STANCE_CROUCH)
		{

			if(sqrt((HEIGHT_OFFSET - 7) * (HEIGHT_OFFSET - 7)) < 0.1)
			{
				HEIGHT_OFFSET = 7;
			}
			else if(HEIGHT_OFFSET > 7)
			{
				HEIGHT_OFFSET -= 8 * dt;
			}
			else if(HEIGHT_OFFSET < 7)
			{
				HEIGHT_OFFSET += 8 * dt;
			}

			float yDiff = target.y - position.y;
			position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);	
			target.y = position.y + yDiff;
		}

		else if(stance == PLAYER_STANCE::STANCE_PRONE)
		{
			if(sqrt((HEIGHT_OFFSET - 5) * (HEIGHT_OFFSET - 5)) < 0.1)
			{
				HEIGHT_OFFSET = 5;
			}
			else if(HEIGHT_OFFSET > 5)
			{
				HEIGHT_OFFSET -= 8 * dt;
			}
			else if(HEIGHT_OFFSET < 5)
			{
				HEIGHT_OFFSET += 8 * dt;
			}

			float yDiff = target.y - position.y;
			position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);	
			target.y = position.y + yDiff;
		}
	}

	if(Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(CAMERA_SPEED_WALK * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED_WALK * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED_WALK * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED_WALK * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}

	//Update the camera direction based on mouse move
	if(Application::camera_yaw != 0)
	{
		Yaw(dt, heightMap, terrainSize);
	}

	if(Application::camera_pitch != 0)
	{
		Pitch(dt, heightMap, terrainSize);
	}

	/*if(Application::IsKeyPressed('R'))
	{
		Reset();
	}*/
}

void Camera3::UpdateStatus(const unsigned char key)
{
	myKeys[key] = true;
}

void Camera3::MoveForward(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 tempTarget = target;
	tempTarget.y = position.y;
	Vector3 view = (tempTarget - position).Normalized();
	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		position += view * CAMERA_SPEED_WALK * (float)dt;
		target += view * CAMERA_SPEED_WALK * (float)dt;
	}
	
	else if(stance == PLAYER_STANCE::STANCE_RUN)
	{
		position += view * CAMERA_SPEED_RUN * (float)dt;
		target += view * CAMERA_SPEED_RUN * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		position += view * CAMERA_SPEED_CROUCH * (float)dt;
		target += view * CAMERA_SPEED_CROUCH * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		position += view * CAMERA_SPEED_PRONE * (float)dt;
		target += view * CAMERA_SPEED_PRONE * (float)dt;
	}

	float yDiff = target.y - position.y;
	if(!m_bJumping)
	{
		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);	
		target.y = position.y + yDiff;
	}
}

void Camera3::MoveBackward(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 tempTarget = target;
	tempTarget.y = position.y;
	Vector3 view = (tempTarget - position).Normalized();
	
	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		position -= view * CAMERA_SPEED_WALK * (float)dt;
		target -= view * CAMERA_SPEED_WALK * (float)dt;
	}
	
	else if(stance == PLAYER_STANCE::STANCE_RUN)
	{
		position -= view * CAMERA_SPEED_RUN * (float)dt;
		target -= view * CAMERA_SPEED_RUN * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		position -= view * CAMERA_SPEED_CROUCH * (float)dt;
		target -= view * CAMERA_SPEED_CROUCH * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		position -= view * CAMERA_SPEED_PRONE * (float)dt;
		target -= view * CAMERA_SPEED_PRONE * (float)dt;
	}

	float yDiff = target.y - position.y;
	if(!m_bJumping)
	{
		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	}
	target.y = position.y + yDiff;
}	

void Camera3::MoveLeft(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	
	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		position -= right * CAMERA_SPEED_WALK * (float)dt;
		target -= right * CAMERA_SPEED_WALK * (float)dt;
	}
	
	else if(stance == PLAYER_STANCE::STANCE_RUN)
	{
		position -= right * CAMERA_SPEED_RUN * (float)dt;
		target -= right * CAMERA_SPEED_RUN * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		position -= right * CAMERA_SPEED_CROUCH * (float)dt;
		target -= right * CAMERA_SPEED_CROUCH * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		position -= right * CAMERA_SPEED_PRONE * (float)dt;
		target -= right * CAMERA_SPEED_PRONE * (float)dt;
	}

	float yDiff = target.y - position.y;
	if(!m_bJumping)
	{
		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	}
	target.y = position.y + yDiff;
}

void Camera3::MoveRight(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		position += right * CAMERA_SPEED_WALK * (float)dt;
		target += right * CAMERA_SPEED_WALK * (float)dt;
	}
	
	else if(stance == PLAYER_STANCE::STANCE_RUN)
	{
		position += right * CAMERA_SPEED_RUN * (float)dt;
		target += right * CAMERA_SPEED_RUN * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		position += right * CAMERA_SPEED_CROUCH * (float)dt;
		target += right * CAMERA_SPEED_CROUCH * (float)dt;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		position += right * CAMERA_SPEED_PRONE * (float)dt;
		target += right * CAMERA_SPEED_PRONE * (float)dt;
	}

	float yDiff = target.y - position.y;
	if(!m_bJumping)
	{
		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	}
	target.y = position.y + yDiff;
}

void Camera3::TurnLeft(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED_WALK * Application::camera_yaw * (float)dt);
	totalyaw += yaw;
	rotation.SetToRotation(yaw,0,1,0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

void Camera3::TurnRight(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED_WALK * Application::camera_yaw * (float)dt);
	totalyaw += yaw;
	rotation.SetToRotation(yaw,0,1,0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}

void Camera3::LookUp(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	float pitch = (float)(-CAMERA_SPEED_WALK * Application::camera_pitch * (float)dt);
	totalpitch += pitch;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}

void Camera3::LookDown(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	float pitch = (float)(-CAMERA_SPEED_WALK * Application::camera_pitch * (float)dt);
	totalpitch += pitch;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;
}

void Camera3::SpinClockWise(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{

}

void Camera3::SpinCounterClockWise(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{

}

void Camera3::Pitch(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(Application::camera_pitch > 0.0)
	{
		LookUp(dt, heightMap, terrainSize);
	}
	else if(Application::camera_pitch < 0.0)
	{
		LookDown(dt, heightMap, terrainSize);
	}
}

void Camera3::Yaw(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(Application::camera_yaw > 0.0)
	{
		TurnRight(dt, heightMap, terrainSize);
	}
	else if(Application::camera_yaw < 0.0)
	{
		TurnLeft(dt, heightMap, terrainSize);
	}
}

void Camera3::Roll(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{

}

void Camera3::Walk(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(dt > 0)
	{
		MoveForward(dt, heightMap, terrainSize);
	}
	else if(dt < 0)
	{
		MoveBackward(abs(dt), heightMap, terrainSize);
	}
}

void Camera3::Strafe(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(sCameraType == LAND_CAM)
	{
		if(dt > 0)
		{
			MoveRight(dt, heightMap, terrainSize);
		}
		else if(dt < 0)
		{
			MoveLeft(abs(dt), heightMap, terrainSize);
		}
	}
}

void Camera3::Jump(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(m_bJumping == false && stance == PLAYER_STANCE::STANCE_STAND)
	{
		m_bJumping = true;
		//Calculate the jump velocity
		JumpVel.y = JumpAccel.y;

		//Factor in maximum speed limit
		if(JumpVel.y > JumpMaxSpeed)
		{
			JumpVel.y = JumpMaxSpeed;
		}
	}
}

void Camera3::UpdateJump(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(m_bJumping == true)
	{
		//Factor in gravity
		JumpVel.y += Gravity.y * dt;

		//Update the camera and target position
		position.y += JumpVel.y * (float)dt;
		target.y += JumpVel.y * (float)dt;

		Vector3 view = target - position;
		//Check if the camera has reached the ground
		if(position.y <= HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z))
		{
			position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
			target = position + view;
			JumpVel.y = 0;
			m_bJumping = false;
		}
	}
}

void Camera3::Run(bool run, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(run)
	{
		stance = PLAYER_STANCE::STANCE_RUN;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}

	else if(!run)
	{
		stance = PLAYER_STANCE::STANCE_STAND;
	}
}

void Camera3::ChangeStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize, double dt)
{
	if(m_bJumping == false && stance != PLAYER_STANCE::STANCE_RUN)
	{
		UpdateStance(heightMap, terrainSize, dt);
	}
}

void Camera3::UpdateStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize, double dt)
{
	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		stance = PLAYER_STANCE::STANCE_CROUCH;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		
		if(sqrt((HEIGHT_OFFSET - 7) * (HEIGHT_OFFSET - 7)) < 0.1)
		{
			HEIGHT_OFFSET = 7;
		}
		else if(HEIGHT_OFFSET > 7)
		{
			HEIGHT_OFFSET -= 8 * dt;
		}
		else if(HEIGHT_OFFSET < 7)
		{
			HEIGHT_OFFSET += 8 * dt;
		}

		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		stance = PLAYER_STANCE::STANCE_PRONE;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();

		if(sqrt((HEIGHT_OFFSET - 5) * (HEIGHT_OFFSET - 5)) < 0.1)
		{
			HEIGHT_OFFSET = 5;
		}
		else if(HEIGHT_OFFSET > 5)
		{
			HEIGHT_OFFSET -= 8 * dt;
		}
		else if(HEIGHT_OFFSET < 5)
		{
			HEIGHT_OFFSET += 8 * dt;
		}

		position.y = HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		stance = PLAYER_STANCE::STANCE_STAND;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();

		if(sqrt((HEIGHT_OFFSET - 10) * (HEIGHT_OFFSET - 10)) < 0.1)
		{
			HEIGHT_OFFSET = 10;
		}
		else if(HEIGHT_OFFSET > 10)
		{
			HEIGHT_OFFSET -= 8 * dt;
		}
		else if(HEIGHT_OFFSET < 10)
		{
			HEIGHT_OFFSET += 8 * dt;
		}

		position.y =  HEIGHT_OFFSET + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}
}

void Camera3::SetCameraType(CAM_TYPE sCameraType)
{
	this->sCameraType = sCameraType;
}

Camera3::CAM_TYPE Camera3::GetCameraType()
{
	return sCameraType;
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}