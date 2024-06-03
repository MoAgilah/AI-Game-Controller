#ifndef BillH
#define BillH

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
	void Die();
	~Bill();
	void Render(sf::RenderWindow & window) final;
	Body GetBody();
private:
	Body colbody;
	void Animate(float deltaTime);
};

#endif
