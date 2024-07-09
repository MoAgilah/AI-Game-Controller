#pragma once
#include "../GameObjects/Enemy.h"

enum PPlantAnims { FALL, JUMP, MAXANIM };

class PPlant : public Enemy
{
public:
	explicit PPlant(const sf::Vector2f& initPos);
	~PPlant() final = default;

	void Reset() final;

	bool GetOnGround() const = delete;
	void SetOnGround(bool grnd) = delete;

	void Die() final;

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
private:

	void Animate(float deltaTime) final;
};