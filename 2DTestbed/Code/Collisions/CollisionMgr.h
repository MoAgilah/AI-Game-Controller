#pragma once
#include <vector>
#include <memory>
#include "../Collisions/Grid.h"
#include "../Collisions/Tile.h"
#include "../GameObjects/Object.h"

class CollisionMgr
{
public:
	CollisionMgr();
	~CollisionMgr() = default;

	void AddCollidable(Object* go);
	void Render(sf::RenderWindow& window);

	void ProcessCollisions();
private:

	Grid m_grid;
	std::vector<std::shared_ptr<Tile>> m_tiles;
	std::vector<std::shared_ptr<Object>> m_collidables;
};