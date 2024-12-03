#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
{
	m_keyStates.fill(false);
}

void InputManager::ProcessKeyPressedEvent(sf::Event& event)
{
	SetKeyPressed(event.key.code);
}

void InputManager::ProcessKeyReleasedEvent(sf::Event& event)
{
	SetKeyReleased(event.key.code);
}

void InputManager::SetKeyPressed(sf::Keyboard::Key key)
{
	SetKeyState(key, true);
	std::cout << " key is pressed" << std::endl;
}

void InputManager::SetKeyReleased(sf::Keyboard::Key key)
{
	SetKeyState(key, false);
	std::cout << " key is released" << std::endl;
}

bool InputManager::IsAnyKeyPressed()
{
	return std::any_of(m_keyStates.begin(), m_keyStates.end(), [](const auto& keyState) { return keyState == true; });
}
