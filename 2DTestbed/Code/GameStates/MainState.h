#pragma once

#include "GameState.h"
#include "../Game/Game.h"
#include <SFML/Graphics.hpp>

class MainState : public GameState
{
public:
	explicit MainState(Game* gameMgr);
	~MainState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	std::shared_ptr<Game> m_gameMgr;
};