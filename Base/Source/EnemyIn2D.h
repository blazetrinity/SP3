#ifndef ENEMYIN2D_H
#define ENEMYIN2D_H

#include "Strategy.h"
#include "CharacterIn2D.h"
#include "SpriteAnimation.h"

class EnemyIn2D : public CharacterIn2D
{
public:
	EnemyIn2D(void);
	~EnemyIn2D(void);

	enum ENEMY_TYPE
	{
		WHITE_GHOST_PATROL_UPDOWN = 2,
		WHITE_GHOST_PATROL_LEFTRIGHT,
		RED_GHOST_PATROL_UPDOWN,
		RED_GHOST_PATROL_LEFTRIGHT,
		BOSS_GHOST,

		NUM_ENEMY_TYPE,
	};

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

	//Init Enemy
	void Init(Vector2 position, Vector2 scale, float mass, int gameLevel, SpriteAnimation *newSprite, ENEMY_TYPE newType, float tileSize);

	//Update Enemy
	void Update(CMap *map, double dt, bool topDown);

	// Set Mesh of the Enemy
	void SetMesh(SpriteAnimation* newSprite);
	// Change Current Animation
	void ChangeAnimation(ANIMATION_TYPE Animation);
	//Set Animations
	void SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time);
	
	// Get Mesh of the player
	SpriteAnimation* GetMesh();
	// Get Current Animation of the player
	ANIMATION_TYPE GetAnimation();

	
	// Set Active
	void SetActive(bool newActive);
	// Set Enemy type
	void SetEnemyType(ENEMY_TYPE newtype);
	
	// Get gamelevel of the enemy
	int GetCurrentLevel();
	
	// Change Strategy
	void ChangeStrategy(Strategy* theNewStrategy, bool bDelete=true);
	// Get CurrentStrategy
	Strategy* GetStrategy();

	// Get Facing
	bool GetFacing();
	// Get Active
	bool GetActive();
	
	// Get Enemy type
	ENEMY_TYPE GetEnemyType();

private:
	// Enemy's gamelevel
	int m_currentLevel;
	// The Destination is the position of the Hero
	Strategy* m_strategy;
	// Enemy's Mesh
	SpriteAnimation *m_sprite;
	// Enemy's current animation
	ANIMATION_TYPE m_currentAnimation;
	// Enemy's list of aniamtions
	Animation *m_animations[NUM_ANIMATION];
	// Active
	bool m_active;
	// Enemy type
	ENEMY_TYPE m_enemyType;
};

#endif