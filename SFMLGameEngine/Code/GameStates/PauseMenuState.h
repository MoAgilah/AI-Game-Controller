#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UI/Menu.h"

class PauseMenuState : public GameState
{
public:
	explicit PauseMenuState(GameManager* gameMgr);
	~PauseMenuState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	Menu m_menu;
	Sprite m_backgroundSpr;
	GameManager* m_gameMgr;
};