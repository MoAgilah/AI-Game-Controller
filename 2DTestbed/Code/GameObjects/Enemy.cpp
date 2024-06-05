#include "../GameObjects/Enemy.h"
#include "../Collisions/Collisions.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(id, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{}

void Enemy::Update(float deltaTime)
{
	if (GetActive())
	{
		if (m_timeLeftActive > 0)
			m_timeLeftActive -= deltaTime;

		if (m_timeLeftActive < 0)
			SetVisible(false);

		if (m_resetAllowed)
			m_resetAllowed = false;

		if (GetIsAlive() || m_bbox->GetID() == (int)TexID::BillBB)
			Animate(deltaTime);

		m_spr->Update(deltaTime);
		if (GetDirection())
		{
			//+
			m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2, m_spr->GetPosition().y));
		}
		else
		{
			//-
			m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2, m_spr->GetPosition().y));
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
	window.draw(*GetBBox()->GetSprite());
}

void Enemy::Reset()
{
	GameObject::Reset();

	m_tillReset = 0;
	m_resetAllowed = false;

	m_airtime = 0;
	m_crouched = false;

	m_maxLives = m_numLives;

	m_timeLeftActive = 0;
}

void Enemy::DecrementLife()
{
	if (--m_numLives == 0)
	{
		Die();
	}
	else
	{
		if (m_bbox->GetID() == (int)TexID::RexBB)
			((Rex*)this)->Change();
	}
}
