#pragma once
#include "../GameObjects/Enemy.h"

class Chuck : public Enemy
{
public:
	Chuck(bool dir, const sf::Vector2f& initPos);
	~Chuck() final = default;

	void Reset() final;

	void Die() final;
private:

	void Animate(float deltaTime) final;

	bool m_goingUp = true;
	float m_waitTime = 0;
};