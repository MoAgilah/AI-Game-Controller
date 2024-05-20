#ifndef CollisionsH
#define CollisionsH

#include <SFML\Graphics.hpp>
#include "../Utils.h"

#include "../Collisions/Tile.h"
#include "../Collisions/BoundingBox.h"
#include "../Collisions/Grid.h"

enum Direction 
{
	LDIR, RDIR, UDIR, DDIR
};

class GameObject;
class Object;
class Camera;
class Player;
class Collisions
{
public:
	static Collisions* Get();
	~Collisions();
	void AddCollidable(GameObject* ngo);
	void RemoveLastAdded();
	void ReplacePlayer(Player* currPlayer);
	GameObject* GetLastAdded();
	void Render(sf::RenderWindow & window);
	void ProcessCollisions(GameObject * object);
	Tile GetTile(int x, int y);
	std::vector<Tile*> GetGrid();
	std::vector<GameObject*> GetCollidables();
private:
	void ColObjectToTile(GameObject* colObj, Tile* tile);
	void PlayerToTile(GameObject* ply, Tile* tile);
	void ObjectToTile(GameObject* enmy, Tile* tile);

	void ColObjectToColObject(GameObject* colObj1, GameObject* colObj2);
	void PlayerToEnemy(GameObject* ply, GameObject* enmy);
	void PlayerToObject(Player* ply, Object* obj);

	void EnemyToEnemy(GameObject* enmy1, GameObject* enmy2);

	int GetDirTravelling(GameObject * obj);
	void QBoxHit(Player* ply, Object* obj);
	void SmashBoxHit(Player* ply, Object* obj);
	void SpinBoxHit(Player* ply, Object* obj);

	bool CircleToRect(sf::CircleShape circle, Player* spr);

	Collisions();
	static Collisions* instance;
	Grid grid;

	std::vector<Tile*> m_grid;
	std::vector<GameObject*> m_collidables;
};

#endif