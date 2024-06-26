#include "../GameObjects/Chuck.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos = sf::Vector2f())
	:Enemy(TexID::Chuck, 6, 2, (int)TexID::ChuckBB, dir, false, initAnim, animSpd)
{
	std::vector<int> cframes{ 1, 2, 1, 1, 1, 1 };
	GetAnimSpr()->SetFrames(cframes);

	SetInitialPosition(initPos);

	SetPosition(GetInitialPosition());
	m_numLives = m_maxLives = 2;
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
	m_timeLeftActive = 0.5f;
	GetAnimSpr()->ChangeAnim(5);
}

void Chuck::Animate(float deltaTime)
{
	m_waitTime += deltaTime;

	if (m_goingUp)
	{
		if (m_waitTime > 0.5f)
		{
			GetAnimSpr()->ChangeAnim(2);
			SetYVelocity(-c_jumpSpeed);
		}
		else
		{
			SetYVelocity(0);
		}
	}
	else
	{
		IncrementYVelocity(c_gravity);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y < 357)
	{
		GetAnimSpr()->ChangeAnim(4);
		if(m_goingUp)
			m_goingUp = false;

	}

	if (currentPos.y > 447)
	{
		if (!m_goingUp)
		{
			GetAnimSpr()->ChangeAnim(3);
			m_waitTime = 0;
			m_goingUp = true;
		}

	}

	if (GetYVelocity() != 0)
		Move(0, GetYVelocity() * FPS * deltaTime);
}