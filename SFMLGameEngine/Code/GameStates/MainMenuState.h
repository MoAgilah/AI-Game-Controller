#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UI/UIElements.h"

enum MenuPosition { Automation, Manual, Max = Manual };

class MainMenuState : public GameState
{
public:
	explicit MainMenuState(GameManager* gameMgr);
	~MainMenuState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	int m_menuPosition = MenuPosition::Automation;
	const int m_maxMenuPosition = MenuPosition::Max;
	bool m_menuMoved = false;
	bool m_canGoDown = true;
	bool m_canGoUp = true;
	Sprite m_backgroundSpr;
	FlashingText m_automationMessage;
	FlashingText m_manualMessage;
	GameManager* m_gameMgr;
};