#pragma once

#include <SFML/Graphics.hpp>
#include "../GameObjects/Enemy.h"

struct Body
{
	sf::CircleShape front;
	sf::RectangleShape back;
};

class Bill : public Enemy
{
public:
	Bill(bool dir, const sf::Vector2f& initPos);
	~Bill() final = default;

	void Render(sf::RenderWindow& window) final;

	bool GetOnGround() const = delete;
	void SetOnGround(bool grnd) = delete;
	bool GetAirbourne() const = delete;
	void SetAirbourne(bool air) = delete;
	float GetAirTime() const = delete;
	void SetAirTime(float val) = delete;
	void IncAirTime(float val) = delete;

	void Die() final { SetTimeLeftActive(2.f); }

	Body GetBody() const { return m_colbody; }

private:
	void Animate(float deltaTime) final;

	Body m_colbody;
};