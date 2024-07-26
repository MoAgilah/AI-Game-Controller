#include "MainState.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/CollisionManager.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"

MainState::MainState(GameManager* gameMgr)
	: GameState("Main")
{
	m_gameMgr.reset(gameMgr);
}

void MainState::Initialise()
{
	auto level = m_gameMgr->GetLevel();
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
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

	m_gameMgr->GetLevel()->Update(deltaTime);
	m_gameMgr->GetPlayer()->Update(deltaTime);
}

void MainState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera()->Reset(window);

	m_gameMgr->CheckInView();

	m_gameMgr->GetLevel()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);

	GameManager::GetGameMgr()->GetCollisionMgr()->Render(window);
}
