#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class GameState
{
public:
	explicit GameState(std::string_view name)
		: m_StateName(name) {}

	std::string_view GetStateName() const { return m_StateName; }
	virtual ~GameState() = default;

	virtual void Initialise() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void ProcessInputs() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);
private:

	std::string m_StateName;
};
