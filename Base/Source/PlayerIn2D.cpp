#include "PlayerIn2D.h"

const static int MAXLIFE = 3;
static float MOVESPEED = 100;

PlayerIn2D::PlayerIn2D(void)
{
	for(int i = 0; i < NUM_ANIMATION; ++i)
	{
		m_animations[i] = NULL;
	}
}


PlayerIn2D::~PlayerIn2D(void)
{
	if(m_sprite != NULL)
	{
		delete m_sprite;
		m_sprite = NULL;
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

//Init Player
void PlayerIn2D::Init(Vector2 position, Vector2 scale, float mass, float tileSize, Skill* skill, int health)
{
	CharacterIn2D::Init(position, scale, mass, tileSize, skill, health);
	for(int i = 0; i < NUM_ANIMATION; ++i)
	{
		if(m_animations[i] == NULL)
		{
			this->m_animations[i] = new Animation();
		}
	}

	m_invunerable = false;
	m_invunerableTimer = 0;
	m_renderPlayer = true;
	m_renderPlayerTimer = 0;
	m_lifes = MAXLIFE;
	MOVESPEED = 100;
	m_activePowerUp.clear();
}

// Update player position based on player's velocity
void PlayerIn2D::Update(CMap *map, double dt, bool topDown)
{
	// Update position
	CharacterIn2D::Update(map, dt, topDown);

	// Update idle animation
	if(m_velocity.Length() == 0)
	{
		if(m_facingNormal.y > 0)
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
		else if((m_facingNormal.x < 0))
		{
			ChangeAnimation(IDLE_LEFT);
			GetMesh()->Update(dt);
		}
	}

	if(m_skill != NULL)
	{
		m_skill->Update(dt);
	}

	vector<PowerUp*>::iterator it = m_activePowerUp.begin();
	while(it != m_activePowerUp.end())
	{
		PowerUp* powerup = (PowerUp*)* it;

		powerup->Update(dt);

		if(powerup->GetLifeTime() <= 0)
		{
			switch(powerup->GetType())
			{
			case PowerUp::FIRE_SPEED_POWER:
				m_skill->SetCoolDown(powerup->GetOriginalValue());
				break;
			case PowerUp::MOVE_SPEED_POWER:
				MOVESPEED = powerup->GetOriginalValue();
				break;
			}

			it = m_activePowerUp.erase(it);
		}
		else
		{
			++it;
		}
	}

	//Update player vunerablity
	if(m_invunerable)
	{
		m_invunerableTimer -= dt;
		m_renderPlayerTimer += dt;
		if(m_invunerableTimer <= 0)
		{
			m_invunerable = false;
			m_invunerableTimer = 0;
			m_renderPlayer = true;
			m_renderPlayerTimer = 0;
		}
		else
		{
			if(m_renderPlayerTimer >= 0.1f)
			{
				m_renderPlayer = !m_renderPlayer;
				m_renderPlayerTimer = 0;
			}
		}
	}
}

bool PlayerIn2D::Attack()
{
	if(m_skill != NULL)
	{
		return m_skill->Use();
	}
	return false;
}


bool PlayerIn2D::TakeDamage(float damage)
{
	if(!m_invunerable)
	{
		m_invunerable = true;
		m_invunerableTimer = 5;
		m_renderPlayer = false;

		if(CharacterIn2D::TakeDamage(damage))
		{
			--m_lifes;
			m_health = 100;
		}

		cout << "Health " << m_health << endl;

		return true;
	}

	return false;
}
int PlayerIn2D::GetHealth(void)
{
	return m_health;
}
int PlayerIn2D::GetLives(void)
{
	return m_lifes;
}
// Heal Health
void PlayerIn2D::HealHealth(float heal)
{
	this->m_health += heal;

	if(m_health > m_maxHealth)
	{
		this->m_health = m_maxHealth;
	}

	cout << "Health " << m_health << endl;
}

// Add power up
void PlayerIn2D::AddPowerUp(PowerUp* newPowerUp)
{
	for(vector<PowerUp*>::iterator it = m_activePowerUp.begin(); it != m_activePowerUp.end(); ++it)
	{
		PowerUp* powerup = (PowerUp*)* it;

		if(powerup->GetType() == newPowerUp->GetType())
		{
			powerup->ResetLifeTime();
			return;
		}
	}

	switch(newPowerUp->GetType())
	{
	case PowerUp::FIRE_SPEED_POWER:
		newPowerUp->StoreOriginalValue(m_skill->GetCoolDown());
		m_skill->SetCoolDown(newPowerUp->GetmodifiedValue());
		break;
	case PowerUp::MOVE_SPEED_POWER:
		newPowerUp->StoreOriginalValue(MOVESPEED);
		MOVESPEED = newPowerUp->GetmodifiedValue();
		break;
	}
	m_activePowerUp.push_back(newPowerUp);
}

// Set Mesh of the player
void PlayerIn2D::SetMesh(SpriteAnimation* newMesh)
{
	this->m_sprite = new SpriteAnimation;
	*(this->m_sprite) = *newMesh;
}

// Change Current Animation
void PlayerIn2D::ChangeAnimation(ANIMATION_TYPE newAnimation)
{
	if(m_currentAnimation != newAnimation)
	{
		this->m_currentAnimation = newAnimation;
		this->m_sprite->m_anim = m_animations[m_currentAnimation];
		m_sprite->Reset();
	}
}

// Set Animations
void PlayerIn2D::SetAnimation(ANIMATION_TYPE Animation, int startFrame,int endFrame, int repeat, float time)
{
	this->m_animations[Animation]->Set(startFrame, endFrame, repeat, time);
}

// Set Player to be invunerable
void PlayerIn2D::SetInvunerable(bool invunerable)
{
	this->m_invunerable = invunerable;
}

// Set Enemy Health
void PlayerIn2D::SetHealth(int health)
{
	CharacterIn2D::SetHealth(health);
}

// Get Mesh of the player
SpriteAnimation* PlayerIn2D::GetMesh()
{
	return m_sprite;
}

// Get Current Animation of the player
PlayerIn2D::ANIMATION_TYPE PlayerIn2D::GetAnimation()
{
	return m_currentAnimation;
}

// Get bool to determine player render
bool PlayerIn2D::GetRenderPlayer()
{
	return m_renderPlayer;
}

// Get Life
int PlayerIn2D::GetLife()
{
	return m_lifes;
}

// Update Velocity Up Down
void PlayerIn2D::MoveUpDown(const bool mode)
{
	if (mode)
	{
		m_velocity.y = MOVESPEED;
	}
	else
	{
		m_velocity.y = -MOVESPEED;
	}
	m_velocity.x = 0;
}

// Update Velocity Left Right
void PlayerIn2D::MoveLeftRight(const bool mode)
{
	if (mode)
	{
		m_velocity.x = -MOVESPEED;
	}
	else
	{
		m_velocity.x = MOVESPEED;
	}
	m_velocity.y = 0;
}

// Update Velocity Jump
void PlayerIn2D::Jump()
{

}

//Calculate Position after entering level
void PlayerIn2D::CalPosition(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, const float tileSize)
{
	if(m_position.x <= leftBorder)
	{
		m_position.x = rightBorder - tileSize;
	}
	else if(m_position.x >= rightBorder)
	{
		m_position.x = leftBorder + tileSize;
	}
	else if(m_position.y <= topBorder)
	{
		m_position.y = bottomBorder - tileSize;
	}
	else if(m_position.y >= bottomBorder)
	{
		m_position.y = topBorder + tileSize;
	}
}
