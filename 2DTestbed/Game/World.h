#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "../GameObjects/Sprite.h"

enum class GUI { NAME, TIME, MAX };

class World
{
public:
	World();
	~World() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();

private:
	void AddEnemies();
	void AddObject();
	void AddGUI();

	void UpdateGUI();

private:
	Sprite m_backgroundSprite;

	// ememies
	// objects

	// GUI
	sf::Font m_font;
	std::array<Sprite, (int)GUI::MAX> m_sprites;
	std::array<sf::Text, (int)GUI::MAX> m_text;
};
