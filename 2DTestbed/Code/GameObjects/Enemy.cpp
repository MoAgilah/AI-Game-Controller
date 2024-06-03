#include "../GameObjects/Enemy.h"
#include "../Collisions/Collisions.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(id, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
	if (GetActive())
	{
		if (m_timeLeftActive > 0)
		{
			m_timeLeftActive -= deltaTime;
		}

		if (m_timeLeftActive < 0)
		{
			SetVisible(false);
		}

		if (m_resetAllowed)
		{
			m_resetAllowed = false;
		}

		if (m_alive || m_curBBox->GetID() == (int)TexID::BillBB)
		{
			Animate(deltaTime);
		}

		m_curSpr->Update(deltaTime, m_direction);
		if (m_direction)
		{
			//+
			m_curBBox->Update(sf::Vector2f(m_spr->GetPosition().x - 2, m_spr->GetPosition().y));
		}
		else
		{
			//-
			m_curBBox->Update(sf::Vector2f(m_spr->GetPosition().x + 2, m_spr->GetPosition().y));
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

void Enemy::Render(sf::RenderWindow & window)
{
	window.draw(*GetSprite());
}

int Enemy::DecrementLife()
{
	if(m_numLives > 0)
	{
		--m_numLives;
		if (m_numLives == 0)
		{
			Die();
		}
		else
		{
			if (m_curBBox->GetID() == REX)
			{

				((Rex*)this)->Change();
			}

		}

	}

	return m_numLives;
}

void Enemy::ResetLives()
{
	m_numLives = m_maxLives;
}

bool Enemy::GetIsAlive() const
{
	return m_alive;
}

int Enemy::GetLives()
{
	return m_numLives;
}

void Enemy::Reset()
{
	GameObject::Reset();

	m_tillReset = 0;
	m_resetAllowed = false;

	m_airtime = 0;
	m_crouched = false;

	m_maxLives = m_numLives;
	m_alive = true;

	m_timeLeftActive = 0;
}
