#include "../GameObjects/Chuck.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos = sf::Vector2f())
	:Enemy(TexID::Chuck, 6, 2, (int)TexID::ChuckBB, dir, false, initAnim, animSpd)
{
	std::vector<int> cframes{ 1, 2, 1, 1, 1, 1 };
	m_curSpr->SetFrames(cframes);

	m_spawnData.m_initialPos = sf::Vector2f(400, 519);

	m_curSpr->SetPosition(m_spawnData.m_initialPos);
	m_numLives = m_maxLives = 2;

	m_airtime = 0;
	m_waitTime = 0;

	m_goingUp = true;
}

Chuck::~Chuck()
{
}

void Chuck::Reset()
{
	Enemy::Reset();
	m_goingUp = false;
	m_waitTime = 0;
}

void Chuck::Die()
{
	m_numLives = 0;
	m_alive = false;
	m_timeLeftActive = 0.5f;
	m_curSpr->ChangeAnim(5);
}

bool Chuck::Jumping()
{
	return m_waitTime > 0 && m_waitTime < 0.5f;
}

void Chuck::Animate(float deltaTime)
{
	m_waitTime += deltaTime;

	if (m_goingUp)
	{
		if (m_waitTime > 0.5f)
		{
			m_curSpr->ChangeAnim(2);
			m_velocity.y = -2;
		}
		else
		{
			m_velocity.y = 0;
		}
	}
	else
	{
		m_velocity.y = 3;
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y < 357)
	{
		m_curSpr->ChangeAnim(4);
		if(m_goingUp) m_goingUp = false;

	}

	if (currentPos.y > 447)
	{
		if (m_goingUp == false)
		{
			m_curSpr->ChangeAnim(3);
			m_waitTime = 0;
			m_goingUp = true;
		}

	}

	if (m_velocity.y != 0)
	{
		m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
	}
}