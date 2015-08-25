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
void EnemyIn2D::Init(Vector2 position, Vector2 scale, float mass, int gameLevel, SpriteAnimation *newSprite, EnemyIn2D::ENEMY_TYPE newType, float tileSize, Skill* skill, int health)
{
	CharacterIn2D::Init(position, scale, mass, tileSize, skill, health);
	this->m_currentLevel = gameLevel;
	this->m_sprite = new SpriteAnimation;
	*(this->m_sprite) = *newSprite;
	this->m_enemyType = newType;
	this->m_active = false;

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
void EnemyIn2D::Update(CMap* m_cMap, double dt, bool topDown, Vector2 m_playerPosition)
{
	// Update enemy's velocity
	if(m_strategy != NULL)
	{
		if(dynamic_cast<Strategy_Patrol*>(m_strategy) != NULL)
		{
			Strategy_Patrol* patrol = dynamic_cast<Strategy_Patrol*>(m_strategy);

			if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
			{
				patrol->Update(m_cMap, &m_position, &m_velocity, &m_facingNormal, &dt);
			}

			else if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT)
			{
				patrol->Update(m_cMap, &m_position, &m_velocity, &m_facingNormal, &dt);
			}
		}
		else
		{
			Strategy_Chase* chase = dynamic_cast<Strategy_Chase*>(m_strategy);

			if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_UPDOWN || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_UPDOWN)
			{
				chase->Update(&m_playerPosition, &m_position, &m_velocity, &dt);
			}

			else if(m_enemyType == EnemyIn2D::WHITE_GHOST_PATROL_LEFTRIGHT || m_enemyType == EnemyIn2D::RED_GHOST_PATROL_LEFTRIGHT)
			{
				chase->Update(&m_playerPosition, &m_position, &m_velocity, &dt);
			}
		}
	}

	// Update enemy's position
	CharacterIn2D::Update(m_cMap, dt, topDown);

	// Update enemy's moving animation
	if(m_velocity.Length() != 0)
	{
		if(m_facingNormal.x < 0)
		{
			ChangeAnimation(WALK_LEFT);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.y > 0)
		{
			ChangeAnimation(WALK_UP);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.y < 0)
		{
			ChangeAnimation(WALK_DOWN);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.x > 0)
		{
			ChangeAnimation(WALK_RIGHT);
			GetMesh()->Update(dt);
		}
	}
	else
	{
		if(m_facingNormal.x < 0)
		{
			ChangeAnimation(IDLE_LEFT);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.y > 0)
		{
			ChangeAnimation(IDLE_UP);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.y < 0)
		{
			ChangeAnimation(IDLE_DOWN);
			GetMesh()->Update(dt);
		}
		else if(m_facingNormal.x > 0)
		{
			ChangeAnimation(IDLE_RIGHT);
			GetMesh()->Update(dt);
		}
	}
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

bool EnemyIn2D::Attack()
{
	if(m_skill != NULL)
	{
		return m_skill->Use();
	}

	return false;
}

bool EnemyIn2D::TakeDamage(float damage)
{
	if(CharacterIn2D::TakeDamage(damage))
	{
		m_active = false;
		return true;
	}

	return false;
}