#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"

class DebugState : public GameState
{
public:
	explicit DebugState(GameManager* gameMgr);
	~DebugState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	GameManager* m_gameMgr;
};