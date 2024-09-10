#include "Rex.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

Rex::Rex(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Rex, sf::Vector2f(10, 26), AnimationData{4, 3, false, 0.5f}, 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAnimSpr()->SetFrames({ 2, 3, 2, 1 });
	GetAABB()->Update(GetPosition());
}

void Rex::Reset()
{
	GetAnimSpr()->ChangeAnim(RexAnims::WALKTALL);
	GetAABB()->Reset(sf::Vector2f(10, 28));
	Enemy::Reset();
	GetAABB()->Update(GetPosition());
}

void Rex::Die()
{
	GetAnimSpr()->ChangeAnim(RexAnims::FLATTEN);
	SetTimeLeftActive(1.f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		GetAnimSpr()->ChangeAnim(RexAnims::TRANSITION);
		GetAABB()->Reset(sf::Vector2f(14, 16));
		GetAABB()->Move((GetDirection() ? -1.f : 1.f) * 3.f, m_heightDiff);
		m_transitioning = true;
		m_squished = true;
	}

	Enemy::DecrementLife();
}

void Rex::Animate(float deltaTime)
{
	AnimatedSprite* animSpr = GetAnimSpr();
	PhysicsController* physCtrl = GetPhysicsController();
	CollisionManager* colMgr = GameManager::GetGameMgr()->GetCollisionMgr();

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
		SetXVelocity(GameConstants::GroundSpeed);
	}
	else
	{
		SetXVelocity(-GameConstants::GroundSpeed);
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
