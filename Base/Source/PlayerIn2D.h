#ifndef PLAYERIN2D_H
#define PLAYERIN2D_H

#include <algorithm>
#include "CharacterIn2D.h"
#include "SpriteAnimation.h"
#include "PowerUp.h"

class PlayerIn2D : public CharacterIn2D
{
public:
	PlayerIn2D(void);
	virtual ~PlayerIn2D(void);

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

		NUM_ANIMATION,
	};

	//Init Player
	void Init(Vector2 position, Vector2 scale, float mass, float tileSize, Skill* skill, int health);

	//UpdatePlayer
	void Update(CMap *map, double dt, bool topDown);

	// Set Mesh of the player
	void SetMesh(SpriteAnimation* newMesh);
	// Change Current Animation
	void ChangeAnimation(ANIMATION_TYPE newAnimation);
	// Set Animations
	void SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time);
	// Set invunerablity
	void SetInvunerable(bool invunerable);
	// Set Player Health
	void SetHealth(int health);
	//return health
	int GetHealth(void);
	//return lives
	int GetLives(void);
	// Get Mesh of the player
	SpriteAnimation* GetMesh();
	// Get Current Animation of the player
	ANIMATION_TYPE GetAnimation();
	// Get bool to determine player render
	bool GetRenderPlayer();
	// Get Life
	int GetLife();

	// Update player velocity based on keypress
	void MoveUpDown(const bool mode);
	void MoveLeftRight(const bool mode);
	void Jump();

	// Attack
	virtual bool Attack();

	// Take Damage
	virtual bool TakeDamage(float damage);
	// Heal Health
	virtual void HealHealth(float heal);

	// Add power up
	void AddPowerUp(PowerUp* newPowerUp);

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
	int m_lifes;

	vector<PowerUp*> m_activePowerUp;
};

#endif