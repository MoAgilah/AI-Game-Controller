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
	GetAnimSpr()->ChangeAnim(PPlantAnims::SINK);
	Enemy::Reset();
}

void PPlant::Die()
{
	// no way to destroy currently, requires fireplant mario
}

void PPlant::Animate(float deltaTime)
{
	auto animSpr = GetAnimSpr();
	animSpr->Update(deltaTime);

	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetYVelocity(c_jumpSpeed * 0.5f);
	}
	else
	{
		SetYVelocity(-c_jumpSpeed);
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		GameManager::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
	}

	if (GetAirbourne())
	{
		SetDirection(false);
		IncAirTime(deltaTime);
	}
	else
	{
		SetDirection(true);
	}

	if (GetPosition().y > GetInitialPosition().y)
	{
		SetAirbourne(true);
	}

	if (GetAirTime() >= c_maxAirTime)
	{
		SetAirbourne(false);
		SetAirTime(0);
	}
}
