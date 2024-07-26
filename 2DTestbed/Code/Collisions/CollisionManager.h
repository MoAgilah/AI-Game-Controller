#pragma once

#include <SFML/Graphics.hpp>


#include "../Collisions/Tile.h"
#include "../Collisions/AABB.h"
#include "../Collisions/Grid.h"

enum Direction
{
	LDIR, RDIR, UDIR, DDIR
};

class Object;
class DynamicObject;
class DynamicCollectable;
class Player;
class Mushroom;
class Enemy;
class QBox;
class SBox;
class Camera;

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

	Tile GetTile(int x, int y);
	std::vector<Tile*> GetGrid();
	std::vector<std::shared_ptr<Object>> GetCollidables();

private:

	void DynamicObjectToTileCollisions(DynamicObject* obj);
	void DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile);

	void ResolveObjectToBoxTop(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box);

	void ColObjectToColObject(Object* colObj1, Object* colObj2);

	void PlayerToEnemy(Player* ply, Enemy* enmy);
	void PlayerToObject(Player* ply, Object* obj);
	void EnemyToEnemy(Enemy* enmy1, Enemy* enmy2);

	int GetDirTravelling(DynamicObject* obj);
	void QBoxHit(Player* ply, QBox* obj);
	void QBoxHit(Mushroom* ply, QBox* obj);
	void SBoxHit(Player* ply, SBox* obj);

	bool CircleToRect(sf::CircleShape circle, Player* spr);

	Grid m_grid;
	std::vector<std::shared_ptr<Tile>> m_tiles;
	std::vector<std::shared_ptr<Object>> m_collidables;
};