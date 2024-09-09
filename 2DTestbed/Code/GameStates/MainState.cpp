#include "MainState.h"
#include "../Collisions/Grid.h"
#include "../Controller/ControllerManager.h"
#include "../Game/Constants.h"
#include "../GameObjects/Player.h"

MainState::MainState(GameManager* gameMgr)
	: GameState("Main")
{
	m_gameMgr.reset(gameMgr);
}

void MainState::Initialise()
{
	auto level = m_gameMgr->GetWorld();
	level->AddEnemies();
	level->AddObjects();
	level->AddForeGroundSprites();
}

void MainState::Pause()
{
}

void MainState::Resume()
{
}

void MainState::ProcessInputs()
{
}

void MainState::Update(float deltaTime)
{
	GameManager::GetGameMgr()->GetTimer().Update(deltaTime);

	if (Automated)
		ControllerManager::GetCtrlMgr()->GetController()->Update();

	m_gameMgr->CheckInView();

	m_gameMgr->GetWorld()->Update(deltaTime);
	m_gameMgr->GetPlayer()->Update(deltaTime);
}

void MainState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera().Reset(window);

	m_gameMgr->GetWorld()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);

	GameManager::GetGameMgr()->GetCollisionMgr()->Render(window);
}
