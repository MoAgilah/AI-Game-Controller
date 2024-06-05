#include "../GameObjects/Rex.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
#include "../Game/Constants.h"

Rex::Rex(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:Enemy(TexID::Rex, 3, 2, (int)TexID::RexBB, dir, false, initAnim, animSpd)
{
	m_spawnData.m_initialPos = initPos;
	SetPosition(m_spawnData.m_initialPos);

	std::vector<int> frames{ 2, 2, 1 };
	m_spr->SetFrames(frames);

	m_numLives = m_maxLives = 2;
}

void Rex::Update(float deltaTime)
{
	if (GetActive())
	{
		if (m_timeLeftActive > 0)
		{
			m_timeLeftActive -= deltaTime;
		}

		if (m_timeLeftActive < 0)
		{
			m_numLives = 0;
		}

		if (m_resetAllowed)
		{
			m_resetAllowed = false;
		}

		if (GetIsAlive())
		{
			Animate(deltaTime);

			m_spr->Update(deltaTime);

			if (GetDirection())
			{
				if (m_numLives == m_maxLives)
				{
					//+
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2.f, m_spr->GetPosition().y));
				}
				else
				{
					//+
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 4.f, m_spr->GetPosition().y + 18.5f));
				}

			}
			else
			{
				if (m_numLives == m_maxLives)
				{
					//+
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2.f, m_spr->GetPosition().y));
				}
				else
				{
					//-
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 4.f, m_spr->GetPosition().y + 18.5f));
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
			if (!Game::GetGameMgr()->GetCamera()->IsInView(m_spawnData.m_initialPos, GetOrigin()))
			{
				Reset();
			}
		}
	}
}

void Rex::Reset()
{
	m_bbox->SetTexture(TexID::RexBB);
	Enemy::Reset();
}

void Rex::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
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
		m_spr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	//check for leftmost and rightmost boundary
	if (m_spr->GetPosition().x < m_spr->GetOrigin().x || m_spr->GetPosition().x > 11776 - m_spr->GetOrigin().x)
	{
		m_spr->Move(-m_velocity.x * FPS * deltaTime, 0);
		SetDirection(!m_direction);
	}

	if (m_velocity.y != 0)
	{
		m_spr->Move(0, m_velocity.y * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}

void Rex::Die()
{
	m_spr->ChangeAnim(2);
	m_timeLeftActive = 0.5f;
}

void Rex::Change()
{
	m_spr->ChangeAnim(1);
	m_bbox->SetTexture(TexID::RexSmlBB);
}