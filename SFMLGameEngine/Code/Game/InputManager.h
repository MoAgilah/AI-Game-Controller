#pragma once

#include <array>
#include "SFML/Window/event.hpp"

class InputManager
{
public:
	InputManager();
	~InputManager() = default;

	void ProcessKeyPressedEvent(sf::Event& event);
	void ProcessKeyReleasedEvent(sf::Event& event);

	bool GetKeyState(int key) { return m_keyStates[key]; }
	void SetKeyState(int key, bool val) { m_keyStates[key] = val; }

	bool IsAnyKeyPressed();

private:

	void SetKeyPressed(sf::Keyboard::Key key);
	void SetKeyReleased(sf::Keyboard::Key key);
	void SetKeyState(sf::Keyboard::Key key, bool state) { m_keyStates[key] = state; }

	std::array<bool, sf::Keyboard::Key::KeyCount> m_keyStates;
};