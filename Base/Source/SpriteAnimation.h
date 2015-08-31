#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "Mesh.h"
#include "ObjectClass.h"
#include <vector>

struct Animation
{
	Animation() {}
	Animation(const Animation &copy)
	{
		this->startFrame = copy.startFrame;
		this->endFrame = copy.endFrame;
		this->repeatCount = copy.repeatCount;
		this->animTime = copy.animTime;
	}

	void Set(int startFrame,int endFrame, int repeat, float time)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
	}

	//Assignment operator
	Animation& operator=(const Animation &copy)
	{
		this->startFrame = copy.startFrame;
		this->endFrame = copy.endFrame;
		this->repeatCount = copy.repeatCount;
		this->animTime = copy.animTime;

		return *this;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
};

class SpriteAnimation : public Mesh , public CObjectClass
{
public:
	SpriteAnimation();
	SpriteAnimation(const SpriteAnimation &copy);
	SpriteAnimation(const std::string & meshName, int row, int col);
	~SpriteAnimation();
	
	SpriteAnimation& operator=(const SpriteAnimation &copy); //Assignment operator
	
	void Update(double dt);
	bool IsFinish();
	void Reset();
	virtual void Render();

	int m_row;
	int m_col;

	float m_currentTime;
	int m_currentFrame;
	int m_playCount;
	Animation *m_anim;
};

#endif