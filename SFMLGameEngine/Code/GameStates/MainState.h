#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"



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

	bool m_ready = false;
	bool m_isPaused = false;
	GameManager* m_gameMgr;
};