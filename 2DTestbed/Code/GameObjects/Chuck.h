#pragma once

#include "../GameObjects/Enemy.h"

enum ChuckAnims { BOUNCE, LEAP, CLAP, HIT, WHIPLASH };

class Chuck : public Enemy
{
public:
	Chuck(bool dir, const sf::Vector2f& initPos);
	~Chuck() final = default;

	void Reset() final;

	void Die() final;

	void DecrementLife() final;

private:

	void Animate(float deltaTime) final;

	float m_waitTime = 0;
	bool m_goingUp = true;
	bool m_tookHit = false;
	Timer m_waitTimer;
};