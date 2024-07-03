#pragma once
#include "../GameObjects/Enemy.h"

class PPlant : public Enemy
{
public:
	explicit PPlant(const sf::Vector2f& initPos);
	~PPlant() final = default;

	void Reset() final;

	bool GetOnGround() const = delete;
	void SetOnGround(bool grnd) = delete;

	void Die() final;
private:

	void Animate(float deltaTime) final;
};