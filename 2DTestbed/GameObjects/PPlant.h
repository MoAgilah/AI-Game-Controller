#ifndef PPlantH
#define	PPlantH

#include "../GameObjects/Enemy.h"

class PPlant :
	public Enemy
{
public:
	PPlant(const sf::Vector2f& initPos);
	void Die();
	~PPlant();
private:
	void Animate(float deltaTime);
};

#endif