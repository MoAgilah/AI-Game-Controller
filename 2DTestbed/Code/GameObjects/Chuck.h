#pragma once
#include "../GameObjects/Enemy.h"

class Chuck :
	public Enemy
{
public:
	Chuck(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos);
	~Chuck() final = default;

	void Reset() final;
	void Die() final;
	bool Jumping() const { return m_waitTime > 0 && m_waitTime < 0.5f; }
private:
	void Animate(float deltaTime) final;

	bool m_goingUp = true;
	float m_waitTime = 0;
};