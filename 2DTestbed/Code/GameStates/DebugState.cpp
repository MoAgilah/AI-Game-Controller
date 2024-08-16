#include "DebugState.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/CollisionManager.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"

DebugState::DebugState(GameManager* gameMgr)
	: GameState("Debug")
{
	m_gameMgr.reset(gameMgr);
	circle.setRadius(2);
	//circle.setOrigin(1, 1);
	circle.setOutlineColor(sf::Color::Red);
	circle.setOutlineThickness(1);
}

void DebugState::Initialise()
{
	auto level = m_gameMgr->GetLevel();

	level->AddObjects();
	//level->AddEnemies();
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
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

	m_gameMgr->CheckInView();

	m_gameMgr->GetPlayer()->Update(deltaTime);
	m_gameMgr->GetLevel()->Update(deltaTime);

	circle.setPosition(m_gameMgr->GetPlayer()->GetAABB()->GetPoint(Side::Bottom));
}

void DebugState::Render(sf::RenderWindow& window)
{
	m_gameMgr->GetCamera()->Reset(window);

	m_gameMgr->GetLevel()->Render(window);


	GameManager::GetGameMgr()->GetCollisionMgr()->Render(window);
	m_gameMgr->GetPlayer()->Render(window);

	window.draw(circle);
	/*m_gameMgr->GetCamera()->RenderViewBox(window);*/
}
