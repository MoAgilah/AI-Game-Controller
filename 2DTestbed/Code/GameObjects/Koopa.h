#pragma once
#include "../GameObjects/Enemy.h"

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

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
private:

	void Animate(float deltaTime) final;
};