#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
	m_keyStates.fill(false);
}

void InputManager::ProcessKeyPressedEvent(sf::Event& event)
{
	if (IsKeyPressed(event.key.code))
	{
		std::cout << " key is pressed" << std::endl;
	}
	else if (IsKeyHeld(event.key.code))
	{
		std::cout << " key is held" << std::endl;
	}
}

void InputManager::ProcessKeyReleasedEvent(sf::Event& event)
{
	if (IsKeyReleased(event.key.code))
	{
		std::cout << " key is released" << std::endl;
	}
}

bool InputManager::IsKeyPressed(sf::Keyboard::Key key)
{
	bool prevKeyState = m_keyStates[key];
	SetKeyState(key, sf::Keyboard::isKeyPressed(key));
	return m_keyStates[key] && !prevKeyState;
}

bool InputManager::IsKeyReleased(sf::Keyboard::Key key)
{
	bool prevKeyState = m_keyStates[key];
	SetKeyState(key, sf::Keyboard::isKeyPressed(key));
	return !m_keyStates[key] && prevKeyState;
}

bool InputManager::IsKeyHeld(sf::Keyboard::Key key)
{
	bool prevKeyState = m_keyStates[key];
	SetKeyState(key, sf::Keyboard::isKeyPressed(key));
	return m_keyStates[key] && prevKeyState;
}

bool InputManager::IsAnyKeyPressed()
{
	return std::any_of(m_keyStates.begin(), m_keyStates.end(), [](const auto& keyState) { return keyState == true; });
}
