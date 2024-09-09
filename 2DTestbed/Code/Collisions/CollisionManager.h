#pragma once

#include <SFML/Graphics.hpp>
#include "../Collisions/AABB.h"
#include "../Collisions/Grid.h"
#include "../Collisions/Tile.h"

enum Direction
{
	LDIR, RDIR, UDIR, DDIR
};

class SBox;
class QBox;
class Enemy;
class Player;
class Object;
class DynamicObject;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager() = default;

	void ProcessCollisions(Object* object);
	void Render(sf::RenderWindow& window);

	void AddCollidable(Object* ngo);
	void RemoveLastAdded();
	void ReplacePlayer(Player* currPlayer);
	Object* GetLastAdded();

	Tile* GetTile(int x, int y);
	std::vector<std::shared_ptr<Tile>> GetGrid();
	std::vector<Object*> GetCollidables();

private:

	void DynamicObjectToTileCollisions(DynamicObject* obj);
	void DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile);

	void PlayerToObjectCollisions(Player* ply, Object* obj);
	void ObjectToObjectCollisions(Object* obj1, Object* obj2);

	void PlayerToQBoxResolutions(Player* ply, QBox* box);
	void PlayerToSBoxResolutions(Player* ply, SBox* box);

	void PlayerToEnemyResolutions(Player* ply, Enemy* enmy);

	void DynamicObjectToBoxResolutions(Direction dirOfTravel, const Point& prevOverlap, DynamicObject* obj, AABB* box, bool resolveUpDir = true);

	void DynamicObjectToDynamicObject(DynamicObject* obj1, DynamicObject* obj2);

	void ResolveObjectToBoxTop(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box);

	Direction GetFacingDirection(DynamicObject* obj);

	Grid m_grid;
	std::vector<Object*> m_collidables;
	std::vector<std::shared_ptr<Tile>> m_tiles;
};