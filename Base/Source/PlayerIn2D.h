#ifndef PLAYERIN2D_H
#define PLAYERIN2D_H

#include "CharacterIn2D.h"
#include "SpriteAnimation.h"

class PlayerIn2D : public CharacterIn2D
{
public:
	PlayerIn2D(void);
	~PlayerIn2D(void);

	enum ANIMATION_TYPE
	{
		IDLE_RIGHT = 0,
		IDLE_LEFT,
		WALK_RIGHT,
		WALK_LEFT,
		JUMP_RIGHT,
		JUMP_LEFT,
		ATTACK_RIGHT,
		ATTACK_LEFT,
		JUMPATTACK_RIGHT,
		JUMPATTACK_LEFT,
		CLIMB,
		NUM_ANIMATION,
	};

	//Init Player
	void Init(Vector2 position, Vector2 scale, float mass, float tileSize);

	//UpdatePlayer
	void Update(CMap *map, double dt, bool topDown);

	// Set Mesh of the player
	void SetMesh(SpriteAnimation* newMesh);
	// Change Current Animation
	void ChangeAnimation(ANIMATION_TYPE Animation);
	//Set Animations
	void SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time);
	
	// Get Mesh of the player
	SpriteAnimation* GetMesh();
	// Get Current Animation of the player
	ANIMATION_TYPE GetAnimation();

	// Update Movements
	void MoveUpDown(const bool mode);
	void MoveLeftRight(const bool mode);
	void Jump();

	//Calculate Position after entering level
	void CalPosition(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float tileSize);

protected:
	ANIMATION_TYPE m_currentAnimation;

	SpriteAnimation* m_sprite;
	Animation *m_animations[NUM_ANIMATION];
};

#endif