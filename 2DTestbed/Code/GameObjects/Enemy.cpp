#include "Enemy.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"

Enemy::Enemy(TexID sprId, const sf::Vector2f& boxSize, int maxLives)
	: DynamicObject(sprId, boxSize), m_numLives(maxLives), m_maxLives(m_numLives)
{
}

Enemy::Enemy(TexID sprId, const sf::Vector2f& boxSize, AnimationData animData, int maxLives)
	: DynamicObject(new AnimatedSprite(sprId, animData.rows, animData.cols, FPS, animData.symmetrical, animData.animationSpeed), boxSize), m_numLives(maxLives), m_maxLives(m_numLives)
{
}

void Enemy::Update(float deltaTime)
{
	if (GetActive())
	{
		Animate(deltaTime);

		if (!HasLifes())
		{
			if (m_timeLeftActive > 0)
				m_timeLeftActive -= deltaTime;

			if (m_timeLeftActive < 0)
				m_isAlive = false;
		}
	}
	else
	{
		if (!m_resetAllowed)
		{
			m_tillReset = 1;
			m_resetAllowed = true;
		}

		if (m_resetAllowed)
		{
			m_tillReset -= deltaTime;
			if (m_tillReset <= 0)
			{
				if (!Game::GetGameMgr()->GetCamera()->IsInView(GetInitialPosition(), GetOrigin()))
				{
					Reset();
				}
			}
		}
	}

}

void Enemy::Reset()
{
	DynamicObject::Reset();

	m_numLives = m_maxLives;
	m_isAlive = true;

	m_tillReset = 0;
	m_resetAllowed = false;

	m_tillReset = 0;
	m_timeLeftActive = 0;
}

void Enemy::DecrementLife()
{
	if (--m_numLives == 0)
		Die();
}