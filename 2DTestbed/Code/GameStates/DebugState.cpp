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
	: GameState("Debug"), m_box{ Collisions::Get()->GetTile(6, 8).GetPosition() }
{
	m_box.SetActive(true);
	m_gameMgr.reset(gameMgr);
}

void DebugState::Initialise()
{
	m_aabb.GetShape()->setOutlineColor(sf::Color::Red);
	auto level = m_gameMgr->GetLevel();
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
	m_gameMgr->GetCamera()->Update();
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

	/*Tile tmp = Collisions::Get()->GetTile(6,8);
	m_aabb.Update(tmp.GetPosition());*/

	m_box.Update(deltaTime);

	m_gameMgr->GetLevel()->Update(deltaTime);
	m_gameMgr->GetPlayer()->Update(deltaTime);
}

void DebugState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera()->Reset(window);
	m_gameMgr->CheckInView();

	m_gameMgr->GetLevel()->Render(window);

	m_box.Render(window);

	m_gameMgr->GetPlayer()->Render(window);

	Collisions::Get()->Render(window);

	/*m_aabb.Render(window);*/
}
