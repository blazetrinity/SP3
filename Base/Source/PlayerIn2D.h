#ifndef PLAYERIN2D_H
#define PLAYERIN2D_H

#include "CharacterIn2D.h"
#include "SpriteAnimation.h"

class PlayerIn2D : public CharacterIn2D
{
public:
	PlayerIn2D(void);
	~PlayerIn2D(void);

	//Types of animations the player has
	enum ANIMATION_TYPE
	{
		IDLE_RIGHT = 0,
		IDLE_LEFT,
		IDLE_UP,
		IDLE_DOWN,
		WALK_RIGHT,
		WALK_LEFT,
		WALK_UP,
		WALK_DOWN,
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
	// Set Animations
	void SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time);
	// Set invunerablity
	void SetInvunerable(bool invunerable);
	
	// Get Mesh of the player
	SpriteAnimation* GetMesh();
	// Get Current Animation of the player
	ANIMATION_TYPE GetAnimation();
	// Get bool to determine player render
	bool GetRenderPlayer();

	// Update player velocity based on keypress
	void MoveUpDown(const bool mode);
	void MoveLeftRight(const bool mode);
	void Jump();

	//Calculate Position after entering level
	void CalPosition(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, float tileSize);

protected:
	ANIMATION_TYPE m_currentAnimation;

	SpriteAnimation* m_sprite;
	Animation *m_animations[NUM_ANIMATION];

	bool m_invunerable;
	float m_invunerableTimer;
	bool m_renderPlayer;
	float m_renderPlayerTimer;
};

#endif