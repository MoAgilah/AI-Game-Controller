#include "DebugState.h"
#include "../Game/Constants.h"

DebugState::DebugState(GameManager* gameMgr)
	: GameState("Debug")
{
	m_gameMgr = gameMgr;
}

void DebugState::Initialise()
{
}

void DebugState::Pause()
{
}

void DebugState::Resume()
{
}

void DebugState::ProcessInputs()
{
}

void DebugState::Update(float deltaTime)
{
}

void DebugState::Render(sf::RenderWindow& window)
{
}
