#ifndef BillH
#define BillH

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
	Bill(bool dir = true);
	void Die();
	~Bill();
	void Render(sf::RenderWindow & window);
	Body GetBody();
private:
	Body colbody;
	void Animate(float deltaTime);
};

#endif
