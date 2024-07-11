#include "DebugState.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"

DebugState::DebugState(Game* gameMgr)
	: GameState("Debug"), m_col{ true, Collisions::Get()->GetTile(4, 11).GetPosition()}, m_col2{ false, Collisions::Get()->GetTile(11,11).GetPosition() }
{
	m_col.SetActive(true);
	m_col2.SetActive(true);
	m_gameMgr.reset(gameMgr);
}

void DebugState::Initialise()
{
	auto level = m_gameMgr->GetLevel();

	level->AddEnemies();
	//level->AddForeGroundSprites();
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
	m_gameMgr->GetCamera()->Update();
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

	/*m_col.Update(deltaTime);
	m_col2.Update(deltaTime);*/

	m_gameMgr->GetLevel()->Update(deltaTime);
	m_gameMgr->GetPlayer()->Update(deltaTime);
}

void DebugState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera()->Reset(window);
	m_gameMgr->CheckInView();

	m_gameMgr->GetLevel()->Render(window);

	/*if (m_col.GetActive())
		m_col.Render(window);

	if (m_col2.GetActive())
		m_col2.Render(window);*/

	m_gameMgr->GetPlayer()->Render(window);

	Collisions::Get()->Render(window);
}
