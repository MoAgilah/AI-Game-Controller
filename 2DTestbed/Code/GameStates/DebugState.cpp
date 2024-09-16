#include "DebugState.h"
#include "../Collisions/Grid.h"
#include "../Controller/ControllerManager.h"
#include "../Game/Constants.h"
#include "../GameObjects/Player.h"

DebugState::DebugState(GameManager* gameMgr)
	: GameState("Debug")
{
	m_gameMgr = gameMgr;
}

void DebugState::Initialise()
{
	auto world = m_gameMgr->GetWorld();
	world->AddObjects();
	world->AddEnemies();
	world->AddForeGroundSprites();
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
	m_gameMgr->GetTimer().Update(deltaTime);

	if (GameConstants::Automated)
		ControllerManager::GetCtrlMgr()->GetController()->Update();

	m_gameMgr->CheckInView();
	m_gameMgr->GetPlayer()->Update(deltaTime);
	m_gameMgr->GetWorld()->Update(deltaTime);
}

void DebugState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera().Reset(window);
	m_gameMgr->GetWorld()->Render(window);
	m_gameMgr->GetCollisionMgr()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);
	//m_gameMgr->GetCamera().RenderViewBox(window);
}
