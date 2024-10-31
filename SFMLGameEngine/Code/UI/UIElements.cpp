#include "UIElements.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"

FlashingText::FlashingText()
	: m_maxTime(0.75), m_timer(m_maxTime)
{
	m_text.setFont(GameManager::Get()->GetFontMgr().GetMenuFont());

	m_textShader.loadFromFile("Resources/Shaders/FadeInOutShader.frag", sf::Shader::Fragment);
}

void FlashingText::Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, float fadeTime)
{
	if (fadeTime != -1.f)
	{
		m_maxTime = fadeTime;
		m_timer.SetTime(m_maxTime);
	}

	m_text.setCharacterSize(charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(charSize / 10.f);
	m_text.setOutlineColor(sf::Color::Black);
	m_text.setPosition(pos);
}

void FlashingText::Update(float deltaTime)
{
	if (m_reduceAlpha)
	{
		m_timer.Update(deltaTime);
		if (m_timer.CheckEnd())
		{
			m_reduceAlpha = false;
		}
	}
	else
	{
		m_timer.Update(-deltaTime);
		if (m_timer.GetTime() >= m_maxTime)
		{
			m_reduceAlpha = true;
			m_timer.ResetTime();
		}
	}

	m_textShader.setUniform("time", m_timer.GetTime() / m_maxTime);
}

void FlashingText::Render(sf::RenderWindow& window)
{
	window.draw(m_text, &m_textShader);
}
