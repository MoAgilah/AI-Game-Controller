#pragma once

#include <array>
#include "SFML/Window/Keyboard.hpp"

class InputManager
{
public:
	InputManager();
	~InputManager() = default;

	bool GetKeyState(sf::Keyboard::Key key) { return m_keyStates[key]; }
	void SetKeyState(sf::Keyboard::Key key, bool state) { m_keyStates[key] = state; }

	bool IsKeyPressed(sf::Keyboard::Key key);
	bool IsKeyReleased(sf::Keyboard::Key key);
	bool IsKeyHeld(sf::Keyboard::Key key);

private:

	std::array<bool, sf::Keyboard::Key::KeyCount> m_keyStates;
};