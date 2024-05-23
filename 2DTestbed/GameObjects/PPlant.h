#ifndef PPlantH
#define	PPlantH

#include "../GameObjects/Enemy.h"

class PPlant :
	public Enemy
{
public:
	PPlant(int initAnim = 0, float animSpd = 1);
	void Die();
	~PPlant();
private:
	void Animate(float deltaTime);
};

#endif