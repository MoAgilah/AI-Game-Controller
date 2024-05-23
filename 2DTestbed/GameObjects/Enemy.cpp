#include "../GameObjects/Enemy.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(std::string_view filepath, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(filepath, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{
	m_enemyNum = s_numOfEnemies++;

	m_initialAnim = initAnim;

	m_CurBox = m_bbox;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
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
				if (!Game::GetGameMgr()->GetCamera()->IsInView(m_initialPos, GetOrigin()))
				{
					Reset();
				}
			}
		}


		if (m_visible)
		{
			if (m_alive || m_bbox->GetID() == BILL)
			{
				Animate(deltaTime);
			}
		}

		m_spr->Update(deltaTime, m_direction);

		if (m_direction)
		{
			//+
			m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x - 2, m_spr->GetPosition().y));
		}
		else
		{
			//-
			m_CurBox->Update(sf::Vector2f(m_spr->GetPosition().x + 2, m_spr->GetPosition().y));
		}
	}
}

void Enemy::Render(sf::RenderWindow & window)
{
	window.draw(*m_spr->GetSprite());
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
			if (m_bbox->GetID() == REX)
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

void Enemy::Change()
{
	return;
}

BoundingBox * Enemy::GetBBox()
{
	return m_CurBox;
}

int Enemy::GetLives()
{
	return m_numLives;
}

void Enemy::Revive()
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

	m_spr->ChangeAnim(m_initialAnim);
	m_alive = true;
	m_active = true;
}

int Enemy::GetEnemyNum()
{
	return m_enemyNum;
}

void Enemy::Reset()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0,0);

	m_resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;

	m_numLives = m_maxLives;

	m_spr->ChangeAnim(m_initialAnim);
}
