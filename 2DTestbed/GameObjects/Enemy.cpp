#include "../GameObjects/Enemy.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(std::string_view filepath, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(filepath, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
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

		if (m_alive || m_curBBox->GetID() == BILL)
		{
			Animate(deltaTime);
		}

		m_curSpr->Update(deltaTime, m_direction);
		m_curSpr->SetPosition(m_curSpr->GetPosition());
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

bool Enemy::GetIsAlive()
{
	return m_alive;
}

int Enemy::GetLives()
{
	return m_numLives;
}

void Enemy::Reset()
{
	m_curBBox = m_bbox.get();
	m_curSpr = m_spr.get();
	m_curSpr->ChangeAnim(m_spawnData.m_initialAnim);

	m_direction = m_spawnData.m_initialDir;
	SetPosition(m_spawnData.m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0,0);

	m_resetAllowed = false;
	m_onGround = false;
	m_airbourne = false;
	m_alive = true;
	m_active = false;

	m_tillReset = 0;
	timeLeftActive = 0;
	m_tillReset = 0;

	m_numLives = m_maxLives;
}
