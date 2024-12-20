#include "Chuck.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Chuck, sf::Vector2f(24, 25), AnimationData{5, 7, false, 0.5f}, 2), m_waitTimer(0)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	GetAnimSpr()->SetFrames({ 3, 1, 1, 7, 3 });

}

void Chuck::Reset()
{
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != ChuckAnims::BOUNCE)
		animSpr->ChangeAnim(ChuckAnims::BOUNCE);
	Enemy::Reset();
}

void Chuck::Die()
{
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != ChuckAnims::WHIPLASH)
		animSpr->ChangeAnim(ChuckAnims::WHIPLASH);
	SetTimeLeftActive(0.5f);
}

void Chuck::DecrementLife()
{
	if (GetInvulnerabe())
		Enemy::DecrementLife();

	if (GetIsAlive())
	{
		m_tookHit = true;
		SetInvulnerability(true);
		auto animSpr = GetAnimSpr();
		if (animSpr->GetCurrentAnim() != ChuckAnims::HIT)
			animSpr->ChangeAnim(ChuckAnims::HIT);
	}
}

void Chuck::Animate(float deltaTime)
{
	AnimatedSprite* animSpr = GetAnimSpr();
	PhysicsController* physCtrl = GetPhysicsController();

	animSpr->Update(deltaTime);

	if (m_tookHit)
	{
		if (GetAirbourne())
		{
			if (physCtrl->GetPhysicsType() != PhysicsType::drop)
				physCtrl->SetFalling();

			IncrementYVelocity(physCtrl->GetYAcceleration());
		}
		else
		{
			if (animSpr->PlayedNumTimes(2))
			{
				m_tookHit = false;
				SetInvulnerability(false);
			}
		}

		if (GetOnGround())
			SetAirbourne(false);
	}
	else
	{
		if (GetAirbourne())
		{
			m_waitTimer.Update(deltaTime);
			if (m_waitTimer.CheckEnd())
			{
				if (physCtrl->GetPhysicsType() != PhysicsType::rise)
					physCtrl->SetAerial();

				DecrementYVelocity(physCtrl->GetYAcceleration());
				GetAirTimer()->Update(deltaTime);
				if (animSpr->GetCurrentAnim() != ChuckAnims::LEAP)
					animSpr->ChangeAnim(ChuckAnims::LEAP);
			}
			else
			{
				SetYVelocity(0);
			}
		}
		else
		{
			if (physCtrl->GetPhysicsType() != PhysicsType::drop)
				physCtrl->SetFalling();

			IncrementYVelocity(physCtrl->GetYAcceleration());
		}

		sf::Vector2f currentPos = GetPosition();

		if (GetAirTimer()->CheckEnd())
		{
			if (animSpr->GetCurrentAnim() != ChuckAnims::CLAP)
				animSpr->ChangeAnim(ChuckAnims::CLAP);
			SetAirbourne(false);
		}

		if (GetOnGround())
		{
			if (!GetAirbourne())
			{
				if (animSpr->GetCurrentAnim() != ChuckAnims::BOUNCE)
					animSpr->ChangeAnim(ChuckAnims::BOUNCE);
				m_waitTimer.SetTime(0.5f);
				SetAirTime(0.75f);
				SetAirbourne(true);
			}
		}
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}
}