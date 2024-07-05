#include "Rex.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
#include "../Game/Constants.h"

Rex::Rex(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Rex, TexID::RexBB, AnimationData{ 3,2,false,0.5f }, 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));

	std::vector<int> frames{ 2, 2, 1 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void Rex::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(0);
	GetBBox()->SetTexture(TexID::RexBB);
	Enemy::Reset();
}

void Rex::Die()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(2);
	SetTimeLeftActive(0.5f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(1);
		GetBBox()->SetTexture(TexID::RexSmlBB);
	}

	Enemy::DecrementLife();
}

void Rex::Animate(float deltaTime)
{
	static_cast<AnimatedSprite*>(GetSprite())->Update(deltaTime);

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
		if (GetDirection())
		{
			GetBBox()->Update(sf::Vector2f(GetPosition().x - 4.f, GetPosition().y + 18.5f));
		}
		else
		{
			GetBBox()->Update(sf::Vector2f(GetPosition().x + 4.f, GetPosition().y + 18.5f));
		}
	}
	else
	{
		Enemy::UpdateBoundingBox();
	}
}
