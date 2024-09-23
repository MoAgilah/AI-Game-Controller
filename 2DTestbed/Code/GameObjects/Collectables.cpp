#include "Collectables.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"

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
	: StaticCollectable(new AnimatedSprite(TexID::Coin, 1, 4, GameConstants::FPS, false, 0.5f), sf::Vector2f(12, 16), initPos)
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
	if (GameConstants::Automated)
		((AutomatedPlayer*)player)->UpdateFitness(5);
}

YCoin::YCoin(const sf::Vector2f& initPos)
	: StaticCollectable(new AnimatedSprite(TexID::YCoin, 1, 6, GameConstants::FPS, false, 0.5f), sf::Vector2f(16, 25), initPos)
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
	if (GameConstants::Automated)
		((AutomatedPlayer*)player)->UpdateFitness(5 * s_collected);
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
	if (GameConstants::Automated)
		((AutomatedPlayer*)player)->UpdateFitness(20);
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
	CollisionManager* colMgr = GameManager::Get()->GetCollisionMgr();
	PhysicsController* physCtrl = GetPhysicsController();

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(GameConstants::ObjectSpeed);
	}
	else
	{
		SetXVelocity(-GameConstants::ObjectSpeed);
	}

	if (GetOnGround())
	{
		SetYVelocity(0);
	}
	else
	{
		if (physCtrl->GetPhysicsType() != PhysicsType::drop)
			physCtrl->SetFalling();

		IncrementYVelocity(GameConstants::Gravity);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		colMgr->ProcessCollisions(this);
	}

	if (GetXVelocity() != 0)
	{
		Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
		colMgr->ProcessCollisions(this);
	}

	CheckForHorizontalBounds(deltaTime);
}

void Mushroom::Collect(Player* player)
{
	SetCollected();
	player->SetIsSuper(true);
	if (GameConstants::Automated)
		((AutomatedPlayer*)player)->UpdateFitness(30);
}

Goal::Goal(const sf::Vector2f& initPos)
	: DynamicCollectable(TexID::Goal, sf::Vector2f(25, 8), initPos), m_airTimer(0)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetOnGround(true);
}

void Goal::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetOnGround())
	{
		m_airTimer.Update(deltaTime);
		SetYVelocity(-GameConstants::ObjectSpeed);
	}
	else
	{
		SetYVelocity(GameConstants::ObjectSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}

	if (m_airTimer.CheckEnd())
	{
		m_airTimer.SetTime(c_maxTravelTime);
		SetOnGround(false);
	}
}

void Goal::Collect(Player* player)
{
	SetCollected();
	player->SetGoalHit(true);
	if (GameConstants::Automated)
		((AutomatedPlayer*)player)->UpdateFitness(30);
}
