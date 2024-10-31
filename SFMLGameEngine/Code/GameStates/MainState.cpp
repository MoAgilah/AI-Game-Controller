#include "MainState.h"
#include "../Game/Constants.h"

MainState::MainState(GameManager* gameMgr)
	: GameState("Main")
{
	m_gameMgr = gameMgr;
}

void MainState::Initialise()
{
	m_gameMgr->InitPlayer();

	auto world = m_gameMgr->GetWorld();
	world->AddEnemies();
	world->AddObjects();
	world->AddForeGroundSprites();
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
	m_gameMgr->GetAIController()->GetGridInputs();

	m_gameMgr->GetTimer().Update(deltaTime);

	if (GameConstants::Automated)
		m_gameMgr->GetAIController()->Update(deltaTime);

	m_gameMgr->CheckInView();

	m_gameMgr->GetWorld()->Update(deltaTime);
	m_gameMgr->GetPlayer()->Update(deltaTime);
}

void MainState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera().Reset(window);

	m_gameMgr->GetWorld()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);

	m_gameMgr->GetCollisionMgr()->Render(window);

	if (GameConstants::DControl)
		m_gameMgr->GetAIController()->GetAnnView()->Render(window);
}
