#include "InputManager.h"

InputManager::InputManager()
{
	m_keyStates.fill(false);
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
