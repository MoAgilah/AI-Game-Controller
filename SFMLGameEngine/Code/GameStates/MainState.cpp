#include "MainState.h"
#include "../Game/Constants.h"
#include "../GameStates/LoadingState.h"

MainState::MainState(GameManager* gameMgr)
	: GameState("Main")
{
	m_gameMgr = gameMgr;
}

void MainState::Initialise()
{
	m_gameMgr->GetGameStateMgr()->PushState(new LoadingState(m_gameMgr));
}

void MainState::Pause()
{
	m_ready = false;
}

void MainState::Resume()
{
	m_ready = true;
}

void MainState::ProcessInputs()
{
}

void MainState::Update(float deltaTime)
{
	if (m_ready)
	{
		m_gameMgr->GetAIController()->GetGridInputs();

		m_gameMgr->GetTimer().Update(deltaTime);

		if (GameConstants::Automated)
			m_gameMgr->GetAIController()->Update(deltaTime);

		m_gameMgr->CheckInView();

		m_gameMgr->GetWorld()->Update(deltaTime);
		m_gameMgr->GetPlayer()->Update(deltaTime);
	}
}

void MainState::Render(sf::RenderWindow& window)
{
	if (m_ready)
	{
		m_gameMgr->GetCamera().Reset(window);

		m_gameMgr->GetWorld()->Render(window);
		m_gameMgr->GetPlayer()->Render(window);

		m_gameMgr->GetCollisionMgr()->Render(window);

		if (GameConstants::DControl)
			m_gameMgr->GetAIController()->GetAnnView()->Render(window);
	}
}