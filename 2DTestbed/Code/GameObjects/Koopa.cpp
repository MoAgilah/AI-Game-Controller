#include "Koopa.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Koopa::Koopa(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Koopa, TexID::KoopaBB, AnimationData{3, 3, false, 0.5f})
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	auto animSpr = GetAnimSpr();
	animSpr->SetFrames({ 2, 3, 1 });
	animSpr->ChangeAnim(KoopaAnims::WALK);
}

void Koopa::Reset()
{
	GetAnimSpr()->ChangeAnim(KoopaAnims::WALK);
	Enemy::Reset();
}

void Koopa::Die()
{
	GetAnimSpr()->ChangeAnim(KoopaAnims::COMPRESS);
	SetTimeLeftActive(0.5f);
}

void Koopa::ResolveCollisions(Object* other)
{
}

void Koopa::ResolveCollisions(Tile* tile)
{
}

void Koopa::Animate(float deltaTime)
{
	GetAnimSpr()->Update(deltaTime);

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
		UpdateBoundingBox();
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
		UpdateBoundingBox();
	}
}
