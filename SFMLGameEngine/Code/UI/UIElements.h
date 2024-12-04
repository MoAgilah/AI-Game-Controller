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

