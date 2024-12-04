#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "../Game/Timer.h"

class FlashingText
{
public:
	FlashingText();
	~FlashingText() = default;

	void Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, float fadeTime = -1.f);

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void Pause();
	void Resume();

private:

	bool m_paused;
	bool m_reduceAlpha = true;
	float m_maxTime;
	Timer m_timer;
	sf::Text m_text;
	sf::Shader* m_textShader;
};

class Menu
{
public:
	Menu(void (*func)(int), const std::string text, unsigned int charSize, unsigned int marginSize, const sf::Vector2f& pos);
	~Menu() = default;

	void AddMenuItem(const std::string text);

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

private:

	bool m_menuMoved = false;
	bool m_canGoDown = true;
	bool m_canGoUp = true;
	int m_menuPosition = 0;
	int m_maxMenuPosition = 0;
	unsigned int m_charSize;
	unsigned int m_marginSize;
	void (*m_actionFunc)(int);
	sf::Vector2f m_position;
	std::vector<FlashingText> m_menuItems;
};