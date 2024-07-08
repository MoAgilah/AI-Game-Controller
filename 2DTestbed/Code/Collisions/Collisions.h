#pragma once

#include <SFML/Graphics.hpp>

#include "../Collisions/Tile.h"
#include "../Collisions/BoundingBox.h"
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

class Collisions
{
public:
	static Collisions* Get();
	~Collisions();
	void AddCollidable(Object* ngo);
	void RemoveLastAdded();
	void ReplacePlayer(Player* currPlayer);
	Object* GetLastAdded();
	void Render(sf::RenderWindow & window);
	void ProcessCollisions(Object* object);
	Tile GetTile(int x, int y);
	std::vector<Tile*> GetGrid();
	std::vector<Object*> GetCollidables();
private:
	void ColObjectToTile(Object* colObj, Tile* tile);
	void PlayerToTile(Player* ply, Tile* tile);
	void ObjectToTile(DynamicObject* enmy, Tile* tile);

	void ColObjectToColObject(Object* colObj1, Object* colObj2);

	void PlayerToEnemy(Player* ply, Enemy* enmy);
	void PlayerToObject(Player* ply, Object* obj);
	void EnemyToEnemy(Enemy* enmy1, Enemy* enmy2);

	int GetDirTravelling(DynamicObject* obj);
	void QBoxHit(Player* ply, QBox* obj);
	void QBoxHit(Mushroom* ply, QBox* obj);
	void SBoxHit(Player* ply, SBox* obj);

	bool CircleToRect(sf::CircleShape circle, Player* spr);

	Collisions();
	static Collisions* instance;
	Grid grid;

	std::vector<Tile*> m_grid;
	std::vector<Object*> m_collidables;
};