#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UI/FlashingText.h"

class TitleState : public GameState
{
public:
	explicit TitleState(GameManager* gameMgr);
	~TitleState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	Sprite m_backgroundSpr;
	FlashingText m_titleMessage;
	GameManager* m_gameMgr;
};