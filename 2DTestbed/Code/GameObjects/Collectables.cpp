#include "Collectables.h"
#include "../Collisions/Collisions.h"

int YCoin::s_collected = 1;

StaticCollectable::StaticCollectable(TexID sprID, TexID boxID, const sf::Vector2f& initPos)
	: Object(sprID, boxID)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
}

StaticCollectable::StaticCollectable(AnimatedSprite* sprite, TexID boxId, const sf::Vector2f& initPos)
	: Object(sprite, boxId)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
}

Coin::Coin(const sf::Vector2f& initPos)
	: StaticCollectable(new AnimatedSprite(TexID::Coin, 4, 1, FPS, false, 0.5f), TexID::CoinBB, initPos)
{
}

void Coin::Update(float deltaTime)
{
	static_cast<AnimatedSprite*>(GetSprite())->Update(deltaTime);
}

int Coin::Collect()
{
	SetActive(false);
	return 1;
}

YCoin::YCoin(const sf::Vector2f& initPos)
	: StaticCollectable(new AnimatedSprite(TexID::YCoin, 6, 1, FPS, false, 0.5f), TexID::YcoinBB, initPos)
{
}

void YCoin::Update(float deltaTime)
{
	static_cast<AnimatedSprite*>(GetSprite())->Update(deltaTime);
}

void YCoin::Reset()
{
	Object::Reset();
	s_collected = 1;
}

int YCoin::Collect()
{
	if (s_collected < 8)
		s_collected *= 2;

	SetActive(false);
	return s_collected * 1000;
}

CheckPoint::CheckPoint(const sf::Vector2f& initPos)
	: StaticCollectable(TexID::ChkPnt, TexID::ChkPntBB, initPos)
{
}

void CheckPoint::Update(float deltaTime)
{
	// nothing to update
}

DynamicCollectable::DynamicCollectable(TexID sprID, TexID boxID, const sf::Vector2f& initPos)
	: DynamicObject(sprID, boxID)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
}

Mushroom::Mushroom(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Shroom, TexID::ShroomBB, initPos)
{
}

void Mushroom::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(2);
	}
	else
	{
		SetXVelocity(-2);
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

Goal::Goal(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Goal, TexID::GoalBB, initPos)
{
}

void Goal::Update(float deltaTime)
{
	if (GetOnGround())
	{
		SetYVelocity(2.5);
	}
	else
	{
		SetYVelocity(-2.5);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
	}

	sf::Vector2f currentPos = GetPosition();
	if (currentPos.y > 470)
		SetOnGround(false);

	if (currentPos.y < 150)
		SetOnGround(true);

	Collisions::Get()->ProcessCollisions(this);
}
