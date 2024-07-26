#include "Rex.h"
#include "../Collisions/CollisionManager.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
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
	SetTimeLeftActive(0.5f);
}

void Rex::DecrementLife()
{
	if (Tall())
	{
		GetAnimSpr()->ChangeAnim(RexAnims::TRANSITION);
		GetAABB()->Reset(sf::Vector2f(14,16));
		GetAABB()->Move((GetDirection() ? -1 : 1) * 3, 12);
		m_transitioning = true;
		m_squished = true;
	}

	Enemy::DecrementLife();
}

void Rex::Animate(float deltaTime)
{
	auto animSpr = GetAnimSpr();
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
		SetXVelocity(1);
	}
	else
	{
		SetXVelocity(-1);
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
		Game::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
	}

	CheckForHorizontalBounds(deltaTime);

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * FPS * deltaTime);
		Game::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
	}
}

void Rex::UpdateBoundingBox()
{
	if (Tall())
	{
		GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	}
	else
	{
		if (GetDirection())
		{
			GetAABB()->Update(sf::Vector2f(GetPosition().x - 6, GetPosition().y + 17));
		}
		else
		{
			GetAABB()->Update(sf::Vector2f(GetPosition().x + 6, GetPosition().y + 17));
		}
	}
}

void Rex::ResolveCollisions(Object* other)
{
}

void Rex::ResolveCollisions(Tile* tile)
{
}
