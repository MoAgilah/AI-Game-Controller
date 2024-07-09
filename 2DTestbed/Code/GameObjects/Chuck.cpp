#include "Chuck.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Chuck, TexID::ChuckBB, AnimationData{ 6, 9, false, 0.5f }, 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));

	std::vector<int> frames{ 7, 3, 1, 5, 9, 3 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void Chuck::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(ChuckAnims::LOOK);
	Enemy::Reset();
}

void Chuck::Die()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(ChuckAnims::WHIPLASH);
}

void Chuck::DecrementLife()
{
	Enemy::DecrementLife();

	if (GetIsAlive())
	{
		m_tookHit = true;
		static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(ChuckAnims::HIT);
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
	auto animSpr = static_cast<AnimatedSprite*>(GetSprite());
	animSpr->Update(deltaTime);

	m_waitTime += deltaTime;

	if (m_tookHit)
	{
		if (animSpr->PlayedNumTimes(2))
		{
			animSpr->ChangeAnim(ChuckAnims::LOOK);
			m_tookHit = false;
		}
	}
	else
	{
		if (m_goingUp)
		{
			if (m_waitTime > 0.5f)
			{
				animSpr->ChangeAnim(ChuckAnims::LEAP);
				SetYVelocity(-c_jumpSpeed);
			}
			else
			{
				SetYVelocity(0);
			}
		}
		else
		{
			IncrementYVelocity(c_gravity);
		}

		sf::Vector2f currentPos = GetPosition();

		if (currentPos.y < 357)
		{
			animSpr->ChangeAnim(ChuckAnims::CLAP);
			if (m_goingUp)
				m_goingUp = false;

		}

		if (currentPos.y > 447)
		{
			if (!m_goingUp)
			{
				animSpr->ChangeAnim(ChuckAnims::LOOK);
				m_waitTime = 0;
				m_goingUp = true;
			}

		}

		if (GetYVelocity() != 0)
		{
			Move(0, GetYVelocity() * FPS * deltaTime);
			Collisions::Get()->ProcessCollisions(this);
		}
	}
}
