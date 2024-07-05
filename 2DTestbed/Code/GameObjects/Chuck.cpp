#include "Chuck.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"

Chuck::Chuck(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Chuck, TexID::ChuckBB, AnimationData{ 6, 2, false, 0.5f })
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));

	std::vector<int> frames{ 1, 2, 1, 1, 1, 1 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void Chuck::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(0);
	Enemy::Reset();
}

void Chuck::Die()
{
}

void Chuck::Animate(float deltaTime)
{
	auto animSpr = static_cast<AnimatedSprite*>(GetSprite());
	animSpr->Update(deltaTime);

	m_waitTime += deltaTime;

	if (m_goingUp)
	{
		if (m_waitTime > 0.5f)
		{
			animSpr->ChangeAnim(2);
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
		animSpr->ChangeAnim(4);
		if (m_goingUp)
			m_goingUp = false;

	}

	if (currentPos.y > 447)
	{
		if (!m_goingUp)
		{
			animSpr->ChangeAnim(3);
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
