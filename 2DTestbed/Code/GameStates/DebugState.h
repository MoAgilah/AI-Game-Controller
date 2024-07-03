#pragma once

#pragma once

#include "GameState.h"
#include "../Game/Game.h"
#include "../Collisions/BoundingVolume.h"
#include "../GameObjects/Box.h"
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
	AABB m_aabb;
	SBox m_box;
	std::shared_ptr<Game> m_gameMgr;
};