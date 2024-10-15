#pragma once

#include <SFML/Graphics.hpp>
#include "../GameObjects/Enemy.h"

struct HalfCapsule
{
	sf::CircleShape circle;
	AABB box;

	bool Intersects(AABB* box);

private:

	bool CircleToAABB(AABB* box);
};

class Bill : public Enemy
{
public:
	Bill(bool dir, const sf::Vector2f& initPos);
	~Bill() final = default;

	void Render(sf::RenderWindow& window) final;
	bool Intersects(Object* obj) final;
	bool IsPlayerAbove(Player* ply) final;

	AnimatedSprite* GetAnimSpr() = delete;
	bool GetOnGround() const = delete;
	void SetOnGround(bool grnd) = delete;
	bool GetAirbourne() const = delete;
	void SetAirbourne(bool air) = delete;
	float GetAirTime() const = delete;
	void SetAirTime(float val) = delete;
	void IncAirTime(float val) = delete;

	void Die() final { SetTimeLeftActive(2.f); }

private:

	void UpdateBody();
	void Animate(float deltaTime) final;

	HalfCapsule m_halfCap;
};