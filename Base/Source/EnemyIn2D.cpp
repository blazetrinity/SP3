#include "EnemyIn2D.h"

EnemyIn2D::EnemyIn2D(void) : m_strategy(NULL)
{
}


EnemyIn2D::~EnemyIn2D(void)
{
	if(m_strategy != NULL)
	{
		delete m_strategy;
		m_strategy = NULL;
	}

	for(int i = 0; i < NUM_ANIMATION; ++i)
	{
		if(m_animations[i] != NULL)
		{
			delete m_animations[i];
			m_animations[i] = NULL;
		}
	}
}

// Initialise this class instance
void EnemyIn2D::Init(Vector2 position, Vector2 scale, float mass, int gameLevel, SpriteAnimation *newSprite, EnemyIn2D::ENEMY_TYPE newType, float tileSize)
{
	CharacterIn2D::Init(position, scale, mass, tileSize);
	this->m_currentLevel = gameLevel;
	this->m_sprite = newSprite;
	this->m_enemyType = newType;
	this->m_active = true;
	if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
	{
		this->m_facingNormal.Set(0, 1);
	}
	else if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT)
	{
		this->m_facingNormal.Set(1, 0);
	}
	for(int i = 0; i < NUM_ANIMATION; ++i)
	{
		this->m_animations[i] = new Animation(); 
	}
}

// Set Mesh of the Enemy
void EnemyIn2D::SetMesh(SpriteAnimation* newSprite)
{
	this->m_sprite = newSprite;
}

// Change Current Animation
void EnemyIn2D::ChangeAnimation(ANIMATION_TYPE Animation)
{
	if(m_currentAnimation != Animation)
	{
		this->m_currentAnimation = Animation;
		m_sprite->m_anim = m_animations[m_currentAnimation];
		m_sprite->Reset();
	}
}

// Set Animations
void EnemyIn2D::SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time)
{
	this->m_animations[Animation]->Set(startFrame, endFrame, repeat, time);
}

// Get Mesh of the player
SpriteAnimation* EnemyIn2D::GetMesh()
{
	return m_sprite;
}

// Get Current Animation of the player
EnemyIn2D::ANIMATION_TYPE EnemyIn2D::GetAnimation()
{
	return m_currentAnimation;
}

// Set Active
void EnemyIn2D::SetActive(bool newActive)
{
	this->m_active = newActive;
}

// Set Enemy type
void EnemyIn2D::SetEnemyType(ENEMY_TYPE newType)
{
	this->m_enemyType = newType;
}

// Get gamelevel of the enemy	
int EnemyIn2D::GetCurrentLevel()
{
	return m_currentLevel;
}

// Update enemy position based on enemy's velocity
void EnemyIn2D::Update(CMap* m_cMap, double dt, bool topDown)
{
	// Update enemy's velocity
	if(m_strategy != NULL)
	{
		if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
		{
			m_strategy->Update(m_cMap, &m_position, &m_velocity, &m_facingNormal, &dt);
		}

		else if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT)
		{
			m_strategy->Update(m_cMap, &m_position, &m_velocity, &m_facingNormal, &dt);
		}
	}

	// Update enemy's position
	CharacterIn2D::Update(m_cMap, dt, topDown);
}

// Update Enemy Ai's strategy
void EnemyIn2D::ChangeStrategy(Strategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (m_strategy != NULL)
		{
			delete m_strategy;
			m_strategy = NULL;
		}
	}

	m_strategy = theNewStrategy;
}

// Get CurrentStrategy
Strategy* EnemyIn2D::GetStrategy()
{
	return m_strategy;
}

// Get Active
bool EnemyIn2D::GetActive()
{
	return m_active;
}

// Get Enemy type
EnemyIn2D::ENEMY_TYPE EnemyIn2D::GetEnemyType()
{
	return m_enemyType;
}
