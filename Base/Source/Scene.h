#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void StartGame() = 0;
	virtual void Render() = 0;
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true) = 0;
	virtual void UpdateWeaponStatus(const unsigned char key) = 0;
	virtual void Exit() = 0;
};

#endif