#include "Rex.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
#include "../Game/Constants.h"

Rex::Rex(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Rex, TexID::RexBB, AnimationData{ 4, 3, false, 0.5f }, 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	std::vector<int> frames{ 2, 3, 2, 1 };
	GetAnimSpr()->SetFrames(frames);

	//DecrementLife();
}

void Rex::Reset()
{
	GetAnimSpr()->ChangeAnim(RexAnims::WALKTALL);
	GetBBox()->SetTexture(TexID::RexBB);
	Enemy::Reset();
}

void Rex::Die()
{
	GetAnimSpr()->ChangeAnim(RexAnims::FLATTEN);
	SetTimeLeftActive(0.5f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		GetAnimSpr()->ChangeAnim(RexAnims::TRANSITION);
		GetBBox()->SetTexture(TexID::RexSmlBB);
		m_transitioning = true;
		m_squished = true;
	}

	Enemy::DecrementLife();
}

void Rex::Animate(float deltaTime)
{
	auto animSpr = GetAnimSpr();
	animSpr->Update(deltaTime);

	if (m_transitioning)
	{
		if (animSpr->PlayedNumTimes(1))
		{
			animSpr->ChangeAnim(RexAnims::WALKSHORT);
			m_transitioning = false;
		}
	}

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
	}
	else
	{
		IncrementYVelocity(c_gravity);
	}

	if (GetXVelocity() != 0)
	{
		Move(GetXVelocity() * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	//check for leftmost and rightmost boundary
	if (GetPosition().x < GetOrigin().x || GetPosition().x > 11776 - GetOrigin().x)
	{
		Move(-GetXVelocity() * FPS * deltaTime, 0);
		SetDirection(!GetDirection());
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}

void Rex::UpdateBoundingBox()
{
	if (Tall())
	{
		GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	}
	else
	{
		if (GetDirection())
		{
			GetBBox()->Update(sf::Vector2f(GetPosition().x - 6, GetPosition().y + 17));
		}
		else
		{
			GetBBox()->Update(sf::Vector2f(GetPosition().x + 6, GetPosition().y + 17));
		}
	}
}

void Rex::ResolveCollisions(Object* other)
{
}

void Rex::ResolveCollisions(Tile* tile)
{
}
