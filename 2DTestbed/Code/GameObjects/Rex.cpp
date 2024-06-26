#include "../GameObjects/Rex.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
#include "../Game/Constants.h"

Rex::Rex(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:Enemy(TexID::Rex, 3, 2, (int)TexID::RexBB, dir, false, initAnim, animSpd)
{
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	std::vector<int> frames{ 2, 2, 1 };
	GetAnimSpr()->SetFrames(frames);

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

			GetAnimSpr()->Update(deltaTime);

			if (GetDirection())
			{
				if (m_numLives == m_maxLives)
				{
					//+
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x - 2.f, GetAnimSpr()->GetPosition().y));
				}
				else
				{
					//+
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x - 4.f, GetAnimSpr()->GetPosition().y + 18.5f));
				}

			}
			else
			{
				if (m_numLives == m_maxLives)
				{
					//+
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x + 2.f, GetAnimSpr()->GetPosition().y));
				}
				else
				{
					//-
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x + 4.f, GetAnimSpr()->GetPosition().y + 18.5f));
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
			if (!Game::GetGameMgr()->GetCamera()->IsInView(GetInitialPosition(), GetOrigin()))
			{
				Reset();
			}
		}
	}
}

void Rex::Reset()
{
	GetBBox()->SetTexture(TexID::RexBB);
	Enemy::Reset();
}

void Rex::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(c_moveSpeed);
	}
	else
	{
		SetXVelocity(-c_moveSpeed);
	}

	if (GetOnGround())
	{
		SetYVelocity(0);
		m_airtime = 0;
	}
	else
	{
		IncrementYVelocity(c_gravity);
	}

	if (GetXVelocity() != 0)
	{
		GetAnimSpr()->Move(GetXVelocity() * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	//check for leftmost and rightmost boundary
	if (GetAnimSpr()->GetPosition().x < GetAnimSpr()->GetOrigin().x || GetAnimSpr()->GetPosition().x > 11776 - GetAnimSpr()->GetOrigin().x)
	{
		GetAnimSpr()->Move(-GetXVelocity() * FPS * deltaTime, 0);
		SetDirection(!GetDirection());
	}

	if (GetYVelocity() != 0)
	{
		GetAnimSpr()->Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}

void Rex::Die()
{
	GetAnimSpr()->ChangeAnim(2);
	m_timeLeftActive = 0.5f;
}

void Rex::Change()
{
	GetAnimSpr()->ChangeAnim(1);
	GetBBox()->SetTexture(TexID::RexSmlBB);
}