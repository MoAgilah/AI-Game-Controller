#pragma once

#include <SFML/Graphics.hpp>
#include "../Utils.h"

#include <array>
#include <vector>
#include <memory>

enum class SPRID { BKG, PIPE1, PIPE2, PIPE3, MAX };

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

	std::vector<Enemy*> GetEnemies();
	std::vector<Object*> GetObjects();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();
private:
	std::array<sf::Sprite, (int)SPRID::MAX> m_sprites;
	std::array<sf::Texture, (int)SPRID::MAX> m_textures;
	std::vector<Enemy*> m_enemy;
	std::vector<Object*> m_objects;
};