#pragma once

#include <memory>
#include <array>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"

enum class Texts { Name, Time, Max };
enum class Sprites { Pipe1, Pipe2, Pipe3, Name, Time, Max};

class World
{
public:
	World();
	~World() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();

	void CheckIsInView();

	void AddObject(const sf::Vector2f& pos);

	void AddEnemies();
	void AddObjects();
	void AddForeGroundSprites();

private:

	void AddGUI();
	void UpdateGUI();

	std::array<sf::Text, (int)Texts::Max> m_texts;
	AABB m_foregroundBox;
	Sprite m_backgroundSpr;
	std::array<Sprite, (int)Sprites::Max> m_sprites;
	std::vector<std::shared_ptr<Enemy>> m_enemies;
	std::vector<std::shared_ptr<Object>> m_objects;
};