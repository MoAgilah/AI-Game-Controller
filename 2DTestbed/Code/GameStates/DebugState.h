#pragma once

#pragma once

#include "GameState.h"
#include "../Game/Game.h"
#include "../Collisions/BoundingVolume.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Collectables.h"
#include "../GameObjects/Chuck.h"
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
	AABB box1;
	AABB box2;
	std::shared_ptr<Game> m_gameMgr;
};