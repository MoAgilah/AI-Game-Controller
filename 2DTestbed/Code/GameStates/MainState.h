#pragma once

#include "GameState.h"
#include "../Game/GameManager.h"
#include <SFML/Graphics.hpp>

class MainState : public GameState
{
public:
	explicit MainState(GameManager* gameMgr);
	~MainState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	std::shared_ptr<GameManager> m_gameMgr;
};