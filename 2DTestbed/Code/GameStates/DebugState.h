#pragma once

#pragma once

#include "GameState.h"
#include "../Game/Game.h"
#include "../Collisions/BoundingVolume.h"
#include "../GameObjects/Collectables.h"
#include <SFML/Graphics.hpp>

class DebugState : public GameState
{
public:
	explicit DebugState(Game* gameMgr);
	~DebugState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:
	Mushroom m_col;
	std::shared_ptr<Game> m_gameMgr;
};