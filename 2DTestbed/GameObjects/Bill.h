#pragma once

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
	Bill(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	void Die();
	~Bill() final = default;
	void Render(sf::RenderWindow & window);
	Body GetBody();
private:
	Body colbody;
	void Animate(float deltaTime);
};
