#include "Menu.h"
#include "../Game/GameManager.h"

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