#include "../GameObjects/Rex.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"

Rex::Rex(bool dir, int initAnim, float animSpd)
	:Enemy("rex.png", 3, 2, REX, dir, false, initAnim, animSpd)
{
	std::vector<int> frames{ 2, 2, 1 };
	m_spr->SetFrames(frames);

	m_type = REX;
	m_numLives = m_maxLives = 2;

	m_CurBox = m_bbox;

	m_SmlBox = new BoundingBox("rexSml", m_type);
}

void Rex::Die()
{
	m_alive = false;
	m_spr->ChangeAnim(2);
	timeLeftActive = 0.5f;
}


void Rex::Change()
{
	m_spr->ChangeAnim(1);

	m_CurBox = m_SmlBox;

	//change bbox
	//smaller one
}

void Rex::Update(float deltaTime)
{
	if (m_active)
	{
		if (m_visible)
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
		}
		//if off screen
		else
		{
			//and wasn't previousily off screen
			if (m_visible != m_prevVisibility)
			{

				m_tillReset = 1;
				m_resetAllowed = true;
			}
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


		if (m_visible)
		{
			if (m_alive)
			{
				Animate(deltaTime);
			}
		}

		m_spr->Update(deltaTime, m_direction);

		if (m_direction)
		{
			if (m_numLives == m_maxLives)
			{
				//+
				m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x - 2.f, m_spr->GetPosition().y));
			}
			else
			{
				//+
				m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x - 4.f, m_spr->GetPosition().y + 18.5f));
			}
		}
		else
		{
			if (m_numLives == m_maxLives)
			{
				//+
				m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x + 2.f, m_spr->GetPosition().y));
			}
			else
			{
				//-
				m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x + 4.f, m_spr->GetPosition().y + 18.5f));
			}
		}
	}
}

void Rex::Reset()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	m_resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;

	m_numLives = m_maxLives;

	m_CurBox = m_bbox;
	m_spr->ChangeAnim(0);
}

void Rex::Revive()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	m_resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;
	timeLeftActive = 0;
	m_numLives = m_maxLives;

	m_CurBox = m_bbox;
	m_spr->ChangeAnim(m_initialAnim);
	m_alive = true;
	m_active = true;
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