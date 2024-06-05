#pragma once

#include <SFML/Graphics.hpp>
#include "../GameObjects/Enemy.h"

struct Body
{
	sf::CircleShape front;
	sf::RectangleShape back;
};

class Bill :
	public Enemy
{
public:
	Bill(bool dir = true, const sf::Vector2f& initPos = sf::Vector2f());
	~Bill() final = default;
	void Render(sf::RenderWindow& window) final;
	void Die() final;

	Body GetBody() const { return m_colbody; }
private:

	void Animate(float deltaTime) final;

	Body m_colbody;
};
