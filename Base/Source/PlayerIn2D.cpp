#include "PlayerIn2D.h"


PlayerIn2D::PlayerIn2D(void)
{
}


PlayerIn2D::~PlayerIn2D(void)
{
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
void PlayerIn2D::Init(Vector2 position, Vector2 scale, float mass, float tileSize)
{
	CharacterIn2D::Init(position, scale, mass, tileSize);
	for(int i = 0; i < NUM_ANIMATION; ++i)
	{
		this->m_animations[i] = new Animation(); 
	}
	
	m_invunerable = false;
	m_invunerableTimer = 0;
	m_renderPlayer = true;
	m_renderPlayerTimer = 0;
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

// Set Mesh of the player
void PlayerIn2D::SetMesh(SpriteAnimation* newMesh)
{
	this->m_sprite = newMesh;
}

// Change Current Animation
void PlayerIn2D::ChangeAnimation(ANIMATION_TYPE Animation)
{
	if(m_currentAnimation != Animation)
	{
		this->m_currentAnimation = Animation;
		m_sprite->m_anim = m_animations[m_currentAnimation];
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

// Update Velocity Up Down
void PlayerIn2D::MoveUpDown(const bool mode)
{
	if (mode)
	{
		m_velocity.y = 100;
	}
	else
	{
		m_velocity.y = -100;
	}
	m_velocity.x = 0;
}

// Update Velocity Left Right
void PlayerIn2D::MoveLeftRight(const bool mode)
{
	if (mode)
	{
		m_velocity.x = -100;
	}
	else
	{
		m_velocity.x = 100;
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
