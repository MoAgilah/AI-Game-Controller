#include "../GameObjects/Koopa.h"
#include "../../Collisions/Collisions.h"


Koopa::Koopa(int rows, int cols, bool dir, bool symmetrical, int initAnim, float animSpd)
	:Enemy("koopa.png", rows, cols, KOOPA, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> frames{ 1, 2, 1};
	m_spr->SetFrames(frames);

	m_spr->SetPosition(sf::Vector2f(400, 524));
	m_type = KOOPA;
	m_numLives = m_maxLives = 1;
}

void Koopa::Die()
{
	m_alive = false;
	m_spr->ChangeAnim(2);
	timeLeftActive = 0.5f;
}


Koopa::~Koopa()
{
}

void Koopa::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (m_direction)
	{
		m_velocity.x = 2;
	}
	else
	{
		m_velocity.x = -2;
	}

	if (m_onGround)
	{
		m_velocity.y = 0;
		m_airtime = 0;
	}
	else
	{
		m_falling = true;
		m_velocity.y += gravity;
	}

	if (m_velocity.x != 0)
	{
		m_spr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}


	//check for leftmost and rightmost boundary
	if (m_spr->GetPosition().x < m_spr->GetOrigin().x || m_spr->GetPosition().x > 11776 - m_spr->GetOrigin().x)
	{
		m_spr->Move(-m_velocity.x * FPS * deltaTime, 0);
		m_direction = !m_direction;
	}

	if (m_velocity.y != 0)
	{
		m_spr->Move(0, m_velocity.y * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}