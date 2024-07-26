#pragma once
#include "../GameObjects/Enemy.h"

enum KoopaAnims { SLIDE, WALK, COMPRESS };

class Koopa : public Enemy
{
public:
	Koopa(bool dir, const sf::Vector2f& initPos);
	~Koopa() final = default;

	void Reset() final;

	bool GetAirbourne() const = delete;
	void SetAirbourne(bool air) = delete;
	float GetAirTime() const = delete;
	void SetAirTime(float val) = delete;
	void IncAirTime(float val) = delete;

	void Die() final;

private:

	void Animate(float deltaTime) final;
};