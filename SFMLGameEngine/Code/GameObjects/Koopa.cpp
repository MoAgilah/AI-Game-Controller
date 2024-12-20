#include "Koopa.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

Koopa::Koopa(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Koopa, sf::Vector2f(17, 12), AnimationData{3, 3, false, 0.5f})
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y));
	auto animSpr = GetAnimSpr();
	animSpr->SetFrames({ 2, 3, 1 });
	animSpr->ChangeAnim(KoopaAnims::WALK);
}

void Koopa::Reset()
{
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != KoopaAnims::WALK)
		animSpr->ChangeAnim(KoopaAnims::WALK);
	Enemy::Reset();
}

void Koopa::Die()
{
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != KoopaAnims::COMPRESS)
		animSpr->ChangeAnim(KoopaAnims::COMPRESS);
	SetTimeLeftActive(0.5f);
}

void Koopa::Animate(float deltaTime)
{
	AnimatedSprite* animSpr = GetAnimSpr();
	PhysicsController* physCtrl = GetPhysicsController();
	CollisionManager* colMgr = GameManager::Get()->GetCollisionMgr();

	animSpr->Update(deltaTime);

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
		if (GetOnSlope())
		{
			if (animSpr->GetCurrentAnim() != KoopaAnims::NOSEDIVE)
				animSpr->ChangeAnim(KoopaAnims::NOSEDIVE);

			if (!GetXVelocity())
			{
				if (GetShouldSlideLeft())
					SetSlideLeft(true);

				if (GetShouldSlideRight())
					SetSlideRight(true);
			}

			if (GetSlideLeft() || GetSlideRight())
			{
				if (GetSlideLeft())
					DecrementXVelocity(physCtrl->GetXAcceleration());

				if (GetSlideRight())
					IncrementXVelocity(physCtrl->GetXAcceleration());
			}
		}
		else
		{
			if (physCtrl->GetPhysicsType() == PhysicsType::drop)
				physCtrl->SetWalking();

			if (animSpr->GetCurrentAnim() == KoopaAnims::NOSEDIVE)
				animSpr->ChangeAnim(KoopaAnims::WALK);

			SetSlideLeft(false);
			SetSlideRight(false);
			SetShouldSlideLeft(false);
			SetShouldSlideRight(false);
		}

		SetYVelocity(0);
	}
	else
	{
		if (physCtrl->GetPhysicsType() != PhysicsType::drop)
			physCtrl->SetFalling();

		IncrementYVelocity(GameConstants::Gravity);
	}

	if (HasLifes())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
			colMgr->ProcessCollisions(this);
		}

		CheckForHorizontalBounds(deltaTime);

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
			colMgr->ProcessCollisions(this);
		}
	}
}