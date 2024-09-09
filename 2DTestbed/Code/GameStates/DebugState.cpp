#include "DebugState.h"
#include "../Collisions/Grid.h"
#include "../Controller/ControllerManager.h"
#include "../Game/Constants.h"
#include "../GameObjects/Player.h"

DebugState::DebugState(GameManager* gameMgr)
	: GameState("Debug")
{
	m_gameMgr.reset(gameMgr);
}

void DebugState::Initialise()
{
	auto level = m_gameMgr->GetLevel();

	level->AddObjects();
	level->AddEnemies();
	level->AddForeGroundSprites();
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
	GameManager::GetGameMgr()->GetTimer()->Update(deltaTime);

	if (Automated)
		ControllerManager::GetCtrlMgr()->GetController()->Update();

	m_gameMgr->CheckInView();

	m_gameMgr->GetPlayer()->Update(deltaTime);
	m_gameMgr->GetLevel()->Update(deltaTime);
}

void DebugState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera()->Reset(window);

	m_gameMgr->GetLevel()->Render(window);

	GameManager::GetGameMgr()->GetCollisionMgr()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);

	sf::Vertex point(m_gameMgr->GetPlayer()->GetAABB()->GetMax(), sf::Color::Black);
	window.draw(&point, 1, sf::Points);
}
