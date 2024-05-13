#pragma once

#include <SFML/Graphics.hpp>
#include "../Utils.h"

#include <memory>
#include <vector>

class Enemy;
class Object;
class Camera;

class Level
{
public:
	Level();
	~Level() = default;

	void AddObject(const std::string& filepath, int rows, int cols, float fps, int bTyp, int strloc, const sf::Vector2f& position, bool onTheGround);

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return m_enemy; }
	const std::vector<std::unique_ptr<Object>>& GetObjects() const { return m_objects; }

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();

private:
	void AddEnemies();
	void AddObjects();

	std::vector<std::unique_ptr<sf::Sprite>> m_sprites;
	std::vector<std::unique_ptr<sf::Texture>> m_textures;
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	std::vector<std::unique_ptr<Object>> m_objects;
};