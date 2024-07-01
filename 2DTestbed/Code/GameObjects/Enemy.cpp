#include "../GameObjects/Enemy.h"
#include "../Collisions/Collisions.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Rex.h"

Enemy::Enemy(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, float animSpd)
	: AnimatedObject(id, bTyp, dir, Cells(rows, cols), symmetrical, animSpd)
{}

void Enemy::Update(float deltaTime)
{
	if (GetActive())
	{
		if (m_timeLeftActive > 0)
			m_timeLeftActive -= deltaTime;

		if (m_timeLeftActive < 0)
			SetActive(false);

		if (m_resetAllowed)
			m_resetAllowed = false;

		if (GetIsAlive() || GetBBox()->GetID() == (int)TexID::BillBB)
			Animate(deltaTime);

		GetAnimSpr()->Update(deltaTime);
		if (GetDirection())
		{
			//+
			GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x - 2, GetAnimSpr()->GetPosition().y));
		}
		else
		{
			//-
			GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x + 2, GetAnimSpr()->GetPosition().y));
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
			if (!Game::GetGameMgr()->GetCamera()->IsInView(GetInitialPosition(), GetOrigin()))
			{
				Reset();
			}
		}
	}
}

void Enemy::Render(sf::RenderWindow & window)
{
	window.draw(*GetAnimSpr()->GetSprite());
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
		if (GetBBox()->GetID() == (int)TexID::RexBB)
			((Rex*)this)->Change();
	}
}
