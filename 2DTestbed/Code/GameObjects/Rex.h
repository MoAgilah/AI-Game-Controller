#pragma once
#include "../GameObjects/Enemy.h"

class Rex : public Enemy
{
public:
	Rex(bool dir, const sf::Vector2f& initPos);
	~Rex() final = default;

	void Reset() final;
	void Die() final;

	bool Tall() const { return !m_squished; }
	void DecrementLife() final;

	void UpdateBoundingBox() final;

private:

	void Animate(float deltaTime) final;

	bool m_squished = false;
};