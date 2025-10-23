#include "PPlant.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

PPlant::PPlant(const sf::Vector2f& initPos)
	: Enemy(TexID::PPlant, sf::Vector2f(14, 19), AnimationData{1, 2, false, 0.5f})
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	GetAnimSpr()->SetFrames({2});
}

void PPlant::Reset()
{
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != PPlantAnims::SINK)
		animSpr->ChangeAnim(PPlantAnims::SINK);
	Enemy::Reset();
}

void PPlant::Die()
{
	// no way to destroy currently, requires fire plant mario, shell toss, or star power not currently implemented
}

void PPlant::Animate(float deltaTime)
{
	AnimatedSprite* animSpr = GetAnimSpr();
	PhysicsController* physCtrl = GetPhysicsController();
	const float step = GameConstants::FPS * deltaTime;

	animSpr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetYVelocity(GameConstants::ObjectSpeed);
	}
	else
	{
		SetYVelocity(-GameConstants::ObjectSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * step);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}

	if (GetAirbourne())
	{
		SetDirection(false);
		GetAirTimer()->Update(deltaTime);
	}
	else
	{
		SetDirection(true);
	}

	if (GetPosition().y > GetInitialPosition().y)
		SetAirbourne(true);

	if (GetAirTimer()->CheckEnd())
	{
		SetAirbourne(false);
		SetAirTime(GameConstants::MaxAirTime+.3f);
	}
}
