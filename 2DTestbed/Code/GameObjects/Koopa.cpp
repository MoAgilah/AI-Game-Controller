#include "../GameObjects/Koopa.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"


Koopa::Koopa(int rows, int cols, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:Enemy(TexID::Koopa, rows, cols, (int)TexID::KoopaBB, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> frames{ 1, 2, 1};
	m_curSpr->SetFrames(frames);

	m_spawnData.m_initialPos = sf::Vector2f(400, 524);
	m_curSpr->SetPosition(m_spawnData.m_initialPos);
	m_type = KOOPA;
	m_numLives = m_maxLives = 1;
}

void Koopa::Die()
{
	m_alive = false;
	m_curSpr->ChangeAnim(2);
	m_timeLeftActive = 0.5f;
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
		m_velocity.y += gravity;
	}

	if (m_velocity.x != 0)
	{
		m_curSpr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}


	//check for leftmost and rightmost boundary
	if (m_curSpr->GetPosition().x < m_curSpr->GetOrigin().x || m_curSpr->GetPosition().x > 11776 - m_curSpr->GetOrigin().x)
	{
		m_curSpr->Move(-m_velocity.x * FPS * deltaTime, 0);
		m_direction = !m_direction;
	}

	if (m_velocity.y != 0)
	{
		m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}