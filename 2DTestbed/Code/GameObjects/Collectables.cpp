#include "Collectables.h"
#include "../Collisions/Collisions.h"

int YCoin::s_collected = 1;

StaticCollectable::StaticCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(sprID, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

StaticCollectable::StaticCollectable(AnimatedSprite* sprite, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(sprite, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

Coin::Coin(const sf::Vector2f& initPos)
	: StaticCollectable(new AnimatedSprite(TexID::Coin, 1,4, FPS, false, 0.5f), sf::Vector2f(12, 16), initPos)
{
	GetAnimSpr()->SetFrames({ 4 });
}

void Coin::Update(float deltaTime)
{
	GetAnimSpr()->Update(deltaTime);
}

int Coin::Collect()
{
	SetActive(false);
	return 1;
}

void Coin::ResolveCollisions(Object* other)
{
}

void Coin::ResolveCollisions(Tile* tile)
{
}

YCoin::YCoin(const sf::Vector2f& initPos)
	: StaticCollectable(new AnimatedSprite(TexID::YCoin, 1,6, FPS, false, 0.5f), sf::Vector2f(16,25), initPos)
{
	GetAnimSpr()->SetFrames({ 6 });
}

void YCoin::Update(float deltaTime)
{
	GetAnimSpr()->Update(deltaTime);
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

void YCoin::ResolveCollisions(Object* other)
{
}

void YCoin::ResolveCollisions(Tile* tile)
{
}

CheckPoint::CheckPoint(const sf::Vector2f& initPos)
	: StaticCollectable(TexID::ChkPnt, sf::Vector2f(20, 3), initPos)
{
}

void CheckPoint::Update(float deltaTime)
{
	// nothing to update
}

void CheckPoint::ResolveCollisions(Object* other)
{
}

void CheckPoint::ResolveCollisions(Tile* tile)
{
}

DynamicCollectable::DynamicCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: DynamicObject(sprID, boxSize)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
}

Mushroom::Mushroom(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Shroom, sf::Vector2f(13, 14), initPos)
{
}

void Mushroom::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());
	UpdateBoundingBox();

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
		UpdateBoundingBox();
	}

	CheckForHorizontalBounds(deltaTime);

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
		UpdateBoundingBox();
	}
}

void Mushroom::ResolveCollisions(Object* other)
{
}

void Mushroom::ResolveCollisions(Tile* tile)
{
}

Goal::Goal(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Goal, sf::Vector2f(25, 8), initPos)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetAirTime(c_maxTravelTime);
}

void Goal::Update(float deltaTime)
{
	UpdateBoundingBox();

	if (GetOnGround())
	{
		IncAirTime(-deltaTime);
		SetYVelocity(-2.5);
	}
	else
	{
		SetYVelocity(2.5);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
		UpdateBoundingBox();
	}

	if (GetAirTime() < 0)
	{
		SetAirTime(c_maxTravelTime);
		SetOnGround(false);
	}
}

void Goal::ResolveCollisions(Object* other)
{
}

void Goal::ResolveCollisions(Tile* tile)
{
}
