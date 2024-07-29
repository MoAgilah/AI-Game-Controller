#include "Collectables.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"
#include "../Collisions/CollisionManager.h"

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

void Coin::Collect(Player* player)
{
	SetCollected();
	player->IncreaseCoins(1);
	//player->UpdateFitness(10);
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

void YCoin::Collect(Player* player)
{
	if (s_collected < 8)
		s_collected *= 2;

	SetCollected();
	player->IncreaseCoins(s_collected * 1000);
}

CheckPoint::CheckPoint(const sf::Vector2f& initPos)
	: StaticCollectable(TexID::ChkPnt, sf::Vector2f(20, 3), initPos)
{
}

void CheckPoint::Update(float deltaTime)
{
	// nothing to update
}

void CheckPoint::Collect(Player* player)
{
	SetCollected();
	player->SetIsSuper(true);
	player->SetSpawnLoc(GetPosition());
}

DynamicCollectable::DynamicCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: DynamicObject(sprID, boxSize)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

Mushroom::Mushroom(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Shroom, sf::Vector2f(13, 10), initPos)
{
}

void Mushroom::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());

	SetXVelocity((GetDirection() ? 1.f : -1.f) * 2);

	if (GetOnGround())
	{
		SetYVelocity(0);
	}
	else
	{
		IncrementYVelocity(c_gravity);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		GameManager::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
	}

	if (GetXVelocity() != 0)
	{
		Move(GetXVelocity() * FPS * deltaTime, 0);
		GameManager::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
	}

	CheckForHorizontalBounds(deltaTime);
}

void Mushroom::Collect(Player* player)
{
	SetCollected();
	player->SetIsSuper(true);
}

Goal::Goal(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Goal, sf::Vector2f(25, 8), initPos)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	//SetAirTime(c_maxTravelTime);
	SetOnGround(false);
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
	{
		SetOnGround(false);
	}

	if (currentPos.y < 150)
	{
		SetOnGround(true);
	}
}

void Goal::Collect(Player* player)
{
	SetCollected();
	player->GoalHit();
}
