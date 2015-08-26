#ifndef ENEMYIN2D_H
#define ENEMYIN2D_H

#include "Strategy_Patrol.h"
#include "Strategy_Chase.h"
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

	enum STRATEGY_TYPE
	{
		PATROL_STRATEGY = 0,
		CHASE_STRATEGY,

		NUM_STRATEGY,
	};

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

	//Init Enemy
	void Init(Vector2 position, Vector2 scale, float mass, int gameLevel, SpriteAnimation *newSprite, ENEMY_TYPE newType, float tileSize, Skill* skill, int health);

	//Update Enemy
	void Update(CMap *map, double dt, bool topDown, Vector2 m_playerPosition);

	// Set Mesh of the Enemy
	void SetMesh(SpriteAnimation* newSprite);
	// Change Current Animation
	void ChangeAnimation(ANIMATION_TYPE Animation);
	// Set Animations
	void SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time);
	// Set Current Strategy
	void SetStrategy(STRATEGY_TYPE newStrategy, AstarPathfind* newPath);
	// Set Active
	void SetActive(bool newActive);
	// Set Enemy type
	void SetEnemyType(ENEMY_TYPE newtype);
	// Set Enemy Health
	virtual void SetHealth(int health);

	// Get Mesh of the player
	SpriteAnimation* GetMesh();
	// Get Current Animation of the player
	ANIMATION_TYPE GetAnimation();
	// Get gamelevel of the enemy
	int GetCurrentLevel();
	// Get CurrentStrategy
	STRATEGY_TYPE GetCurrentStrategy();
	// Get Active
	bool GetActive();
	// Get Spawn Location
	Vector2 GetSpawnLocation();
	// Get View Position
	Vector2 GetViewPosition();
	// Get Enemy type
	ENEMY_TYPE GetEnemyType();

	// Attack
	virtual bool Attack();
	// Take Damage
	virtual bool TakeDamage(float damage);
	// Change Strategy
	void ChangeStrategy(Strategy* theNewStrategy, bool bDelete=true);

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
	// Spawn Position
	Vector2 m_spawnPosition;
	// Enemy current strategy
	STRATEGY_TYPE m_currentStrategy;
};

#endif