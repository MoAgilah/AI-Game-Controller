#include "UIElements.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>

FlashingText::FlashingText()
	: m_paused(false), m_maxTime(0.75), m_timer(m_maxTime)
{
	m_text.setFont(GameManager::Get()->GetFontMgr().GetMenuFont());

	m_textShader = new sf::Shader();
	if (!m_textShader->loadFromFile("Resources/Shaders/FadeInOutShader.frag", sf::Shader::Fragment))
	{
		std::cout << "Resources/Shaders/FadeInOut.frag" << std::endl;
	}
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

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin((textRect.left + textRect.width) / 2.0f,
		(textRect.top + textRect.height) / 2.0f);

	m_text.setPosition(pos);
}

void FlashingText::Update(float deltaTime)
{
	if (m_paused)
	{
		m_textShader->setUniform("time", 1.f);
	}
	else
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

		m_textShader->setUniform("time", m_timer.GetTime() / m_maxTime);
	}
}

void FlashingText::Render(sf::RenderWindow& window)
{
	window.draw(m_text, m_textShader);
}

void FlashingText::Pause()
{
	m_paused = true;
}

void FlashingText::Resume()
{
	m_paused = false;
}

Menu::Menu(void(*func)(int), const std::string text, unsigned int charSize, unsigned int marginSize, const sf::Vector2f& pos)
	: m_actionFunc(func), m_charSize(charSize), m_marginSize(m_charSize + marginSize), m_position(pos)
{
	FlashingText menuItem;
	menuItem.Init(text, charSize, m_position);
	m_menuItems.push_back(menuItem);
	m_position.y += m_marginSize;
}

void Menu::AddMenuItem(const std::string text)
{
	FlashingText menuItem;
	menuItem.Init(text, m_charSize, m_position);
	menuItem.Pause();
	m_menuItems.push_back(menuItem);
	m_position.y += m_marginSize;
	m_maxMenuPosition++;
}

void Menu::ProcessInput()
{
	auto& inputManager = GameManager::Get()->GetInputManager();

	if (inputManager.GetKeyState(sf::Keyboard::Up))
	{
		if (m_canGoUp)
		{
			m_menuPosition--;
			m_canGoUp = false;
			m_menuMoved = true;
		}
	}
	else
	{
		m_canGoUp = true;
	}

	if (inputManager.GetKeyState(sf::Keyboard::Down))
	{
		if (m_canGoDown)
		{
			m_menuPosition++;
			m_canGoDown = false;
			m_menuMoved = true;
		}
	}
	else
	{
		m_canGoDown = true;
	}

	if (inputManager.GetKeyState(sf::Keyboard::Enter))
	{
		m_actionFunc(m_menuPosition);
	}
}

void Menu::Update(float deltaTime)
{
	if (m_menuMoved)
	{
		if (m_menuPosition < 0)
		{
			m_menuPosition = 0;
		}

		if (m_menuPosition >= m_maxMenuPosition)
		{
			m_menuPosition = m_maxMenuPosition;
		}

		for (auto& menuItem : m_menuItems)
			menuItem.Pause();

		m_menuItems[m_menuPosition].Resume();

		m_menuMoved = false;
	}

	for (auto& menuItem : m_menuItems)
		menuItem.Update(deltaTime);
}

void Menu::Render(sf::RenderWindow& window)
{
	for (auto& menuItem : m_menuItems)
		menuItem.Render(window);
}
