#pragma once

#include <memory>
#include <array>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"

enum class GUI { NAME, TIME, MAX };

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
	Sprite m_backgroundSprite;
	std::vector<std::pair<int,Sprite>> m_foregroundSprites;

	std::vector<std::unique_ptr<Enemy>> m_enemies;
	std::vector<std::unique_ptr<Object>> m_objects;

	// GUI
	sf::Font m_font;
	std::array<Sprite, (int)GUI::MAX> m_sprites;
	std::array<sf::Text, (int)GUI::MAX> m_text;
};
