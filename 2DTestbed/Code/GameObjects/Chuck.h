#pragma once
#include "../GameObjects/Enemy.h"

enum ChuckAnims { LOOK, JUMP, CLAP, RUN, HIT, DIE, MAXANIM };

class Chuck : public Enemy
{
public:
	Chuck(bool dir, const sf::Vector2f& initPos);
	~Chuck() final = default;

	void Reset() final;

	void Die() final;

	void DecrementLife() final;

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
private:

	void Animate(float deltaTime) final;

	float m_waitTime = 0;
	bool m_goingUp = true;
	bool m_tookHit = false;
};