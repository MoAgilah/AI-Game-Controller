#include "../GameObjects/Enemy.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	m_enemyNum = s_numOfEnemies++;
	m_tillReset = 0;
	resetAllowed = false;
	m_alive = true;

	m_initialAnim = initAnim;

	m_curBbox = m_bbox.get();
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

			if (resetAllowed)
			{
				resetAllowed = false;
			}
		}
		//if off screen
		else
		{
			//and wasn't previousily off screen
			if (m_visible != m_prevVisibility)
			{

				m_tillReset = 1;
				resetAllowed = true;
			}
		}


		if (resetAllowed)
		{
			m_tillReset -= deltaTime;
			if (m_tillReset <= 0)
			{
				if (!Camera::GetCamera()->IsInView(m_initialPos,GetOrigin()))
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
			m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x - 2, m_spr->GetSpr()->getPosition().y));
		}
		else
		{
			//-
			m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x + 2, m_spr->GetSpr()->getPosition().y));
		}
	}
}

void Enemy::Render(sf::RenderWindow & window)
{
	window.draw(*m_spr->GetSpr());
}

int Enemy::DecrementLife()
{
	if(numLives > 0)
	{
		--numLives;
		if (numLives == 0)
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

	return numLives;
}

void Enemy::ResetLives()
{
	numLives = maxLives;
}

bool Enemy::GetIsAlive()
{
	return m_alive;
}

void Enemy::Change()
{
	return;
}

int Enemy::GetLives()
{
	return numLives;
}

void Enemy::Revive()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;
	timeLeftActive = 0;
	numLives = maxLives;

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

	resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;

	numLives = maxLives;

	m_spr->ChangeAnim(m_initialAnim);
}
