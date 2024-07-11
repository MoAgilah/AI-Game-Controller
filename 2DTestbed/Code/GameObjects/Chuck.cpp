#include "Chuck.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Chuck, TexID::ChuckBB, AnimationData{ 5, 7, false, 0.5f }, 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	GetAnimSpr()->SetFrames({ 3, 1, 1, 7, 3 });
}

void Chuck::Reset()
{
	GetAnimSpr()->ChangeAnim(ChuckAnims::BOUNCE);
	Enemy::Reset();
}

void Chuck::Die()
{
	GetAnimSpr()->ChangeAnim(ChuckAnims::WHIPLASH);
	SetTimeLeftActive(0.5f);
}

void Chuck::DecrementLife()
{
	Enemy::DecrementLife();

	if (GetIsAlive())
	{
		m_tookHit = true;
		GetAnimSpr()->ChangeAnim(ChuckAnims::HIT);
	}
}

void Chuck::ResolveCollisions(Object* other)
{
}

void Chuck::ResolveCollisions(Tile* tile)
{
}

void Chuck::Animate(float deltaTime)
{
	auto animSpr = GetAnimSpr();
	animSpr->Update(deltaTime);

	m_waitTime += deltaTime;

	if (GetAirbourne())
	{
		if (m_waitTime > 0.5f)
		{
			SetYVelocity(-c_jumpSpeed);
			animSpr->ChangeAnim(ChuckAnims::LEAP);
		}
		else
		{
			SetYVelocity(0);
		}
	}
	else
	{
		SetYVelocity(c_jumpSpeed);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y < 325)
	{
		animSpr->ChangeAnim(ChuckAnims::CLAP);
		SetAirbourne(false);
	}

	if (currentPos.y > GetInitialPosition().y)
	{
		if (!GetAirbourne())
		{
			animSpr->ChangeAnim(ChuckAnims::BOUNCE);
			m_waitTime = 0;
			SetAirbourne(true);
		}
	}

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		/*Collisions::Get()->ProcessCollisions(this);*/
		/*UpdateBoundingBox();*/
	}
}
