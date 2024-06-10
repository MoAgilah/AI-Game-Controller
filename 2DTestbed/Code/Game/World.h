#pragma once

#include <memory>
#include <array>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"

enum class Texts { Name, Time, Max };
enum class Sprites { BackGround, Pipe1, Pipe2, Pipe3, Name, Time, Max};

class Camera;

class World
{
public:
	World();
	~World() = default;

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return m_enemies; }
	const std::vector<std::unique_ptr<Object>>& GetObjects() const { return m_objects; }

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();

	void AddObject(const sf::Vector2f& pos);
private:
	void AddEnemies();
	void AddObjects();
	void AddForeGroundSprites();
	void AddGUI();

	void UpdateGUI();

private:

	sf::Font m_font;
	std::array<sf::Text, (int)Texts::Max> m_texts;
	std::array<Sprite, (int)Sprites::Max> m_sprites;
	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<std::unique_ptr<Object>> m_objects;
};