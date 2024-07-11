#include "PPlant.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

PPlant::PPlant(const sf::Vector2f& initPos)
	: Enemy(TexID::PPlant, TexID::PPlantBB, AnimationData{ 1, 2, false, 0.5f })
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
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

void PPlant::ResolveCollisions(Object* other)
{
}

void PPlant::ResolveCollisions(Tile* tile)
{
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
		Collisions::Get()->ProcessCollisions(this);
		UpdateBoundingBox();
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > GetInitialPosition().y)
	{
		SetDirection(false);
	}

	if (currentPos.y < GetInitialPosition().y - 230)
	{
		SetDirection(true);
	}
}
