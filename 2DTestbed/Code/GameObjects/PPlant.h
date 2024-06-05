#ifndef PPlantH
#define	PPlantH

#include "../GameObjects/Enemy.h"

class PPlant :
	public Enemy
{
public:
	explicit PPlant(const sf::Vector2f& initPos);
	~PPlant() final = default;

private:
	void Animate(float deltaTime) final;
};

#endif