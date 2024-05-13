#include "../GameObjects/Chuck.h"
#include "../../Collisions/Collisions.h"


Chuck::Chuck(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:Enemy(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> cframes{ 1, 2, 1, 1, 1, 1 };
	m_spr->SetFrames(cframes);

	m_spr->GetSpr()->setPosition(sf::Vector2f(400, 519));
	m_type = CHUCK;
	numLives = maxLives = 2;

	m_airtime = 0;
	m_waitTime = 0;

	goingUp = true;
}

void Chuck::Die()
{
	numLives = 0;
	m_alive = false;
	timeLeftActive = 0.5f;
	m_spr->ChangeAnim(5);
}

bool Chuck::Jumping()
{
	return m_waitTime > 0 && m_waitTime < 0.5f;
}

void Chuck::Animate(float deltaTime)
{
	m_waitTime += deltaTime;

	if (goingUp)
	{
		if (m_waitTime > 0.5f)
		{
			m_spr->ChangeAnim(2);
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
		m_spr->ChangeAnim(4);
		if(goingUp) goingUp = false;

	}

	if (currentPos.y > 447)
	{
		if (goingUp == false)
		{
			m_spr->ChangeAnim(3);
			m_waitTime = 0;
			goingUp = true;
		}

	}




	if (m_velocity.y != 0)
	{
		m_spr->Move(0, m_velocity.y * FPS * deltaTime);
	}
}