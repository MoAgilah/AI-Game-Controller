#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UI/UIElements.h"

enum MenuPosition { Resume, ToTitle, Quit, Max = Quit };

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

	int m_menuPosition = MenuPosition::Resume;
	const int m_maxMenuPosition = MenuPosition::Max;
	bool m_menuMoved = false;
	Sprite m_backgroundSpr;
	FlashingText m_resumeMessage;
	FlashingText m_toTitleMessage;
	FlashingText m_quitMessage;
	GameManager* m_gameMgr;
};