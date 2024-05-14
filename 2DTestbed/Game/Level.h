#ifndef LevelH
#define LevelH

#include <SFML/Graphics.hpp>
#include "../Utils.h"

#include <vector>

class Enemy;
class Object;
class Camera;
class Level
{
public:
	Level();
	~Level();
	void AddEnemies();
	void AddObjects();
	void AddObject(Object* obj);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();
	std::vector<Enemy*> GetEnemies();
	std::vector<Object*> GetObjects();
private:
	std::vector<sf::Sprite*> m_sprites;
	std::vector<sf::Texture*> m_textures;
	std::vector<Enemy*> m_enemy;
	std::vector<Object*> m_objects;
};

#endif