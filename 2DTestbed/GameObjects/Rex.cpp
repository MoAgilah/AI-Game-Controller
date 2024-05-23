#include "../GameObjects/Rex.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"

Rex::Rex(bool dir, int initAnim, float animSpd)
	:Enemy("rex.png", 3, 2, REX, dir, false, initAnim, animSpd)
{
	std::vector<int> frames{ 2, 2, 1 };
	m_curSpr->SetFrames(frames);

	m_type = REX;
	m_numLives = m_maxLives = 2;

	m_SmlBox = new BoundingBox("rexSml", m_type);
}

void Rex::Die()
{
	m_alive = false;
	m_curSpr->ChangeAnim(2);
	timeLeftActive = 0.5f;
}


void Rex::Change()
{
	m_curSpr->ChangeAnim(1);

	m_curBBox = m_SmlBox;

	//change bbox
	//smaller one
}

void Rex::Update(float deltaTime)
{
	if (m_active)
	{
		if (timeLeftActive > 0)
		{
			timeLeftActive -= deltaTime;
		}

		if (timeLeftActive < 0)
		{
			m_active = false;
		}

		if (m_resetAllowed)
		{
			m_resetAllowed = false;
		}

		if (m_alive)
		{
			Animate(deltaTime);

			m_curSpr->Update(deltaTime, m_direction);

			if (m_direction)
			{
				if (m_numLives == m_maxLives)
				{
					//+
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 2.f, m_curSpr->GetPosition().y));
				}
				else
				{
					//+
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 4.f, m_curSpr->GetPosition().y + 18.5f));
				}
			}
			else
			{
				if (m_numLives == m_maxLives)
				{
					//+
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 2.f, m_curSpr->GetPosition().y));
				}
				else
				{
					//-
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 4.f, m_curSpr->GetPosition().y + 18.5f));
				}
			}
		}
	}
	//if off screen
	else
	{
		m_tillReset = 1;
		m_resetAllowed = true;
	}

	if (m_resetAllowed)
	{
		m_tillReset -= deltaTime;
		if (m_tillReset <= 0)
		{
			if (!Game::GetGameMgr()->GetCamera()->IsInView(m_initialPos,GetOrigin()))
			{
				Reset();
			}
		}
	}
}

void Rex::Reset()
{
	m_curBBox = m_bbox.get();
	m_curSpr = m_spr.get();
	m_curSpr->ChangeAnim(m_initialAnim);

	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	m_resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_active = false;

	m_tillReset = 0;

	m_numLives = m_maxLives;
}

void Rex::Revive()
{
	m_curBBox = m_bbox.get();
	m_curSpr = m_spr.get();
	m_curSpr->ChangeAnim(m_initialAnim);

	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	m_resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_active = false;

	m_tillReset = 0;
	timeLeftActive = 0;
	m_numLives = m_maxLives;

	m_alive = true;
}

Rex::~Rex()
{
}

void Rex::Animate(float deltaTime)
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