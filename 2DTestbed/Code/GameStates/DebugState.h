#pragma once

#pragma once

#include "GameState.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Collectables.h"
#include "../GameObjects/Chuck.h"
#include <SFML/Graphics.hpp>

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
	sf::CircleShape circle;
	std::shared_ptr<GameManager> m_gameMgr;
};