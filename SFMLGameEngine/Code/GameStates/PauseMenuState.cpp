#include "PauseMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/TitleState.h"

enum MenuPosition { Resume, ToTitle, Quit };

void PauseMenuActionFunc(int menuPosition)
{
	auto gameMgr = GameManager::Get();

	switch (menuPosition)
	{
	case MenuPosition::Resume:
		gameMgr->GetGameStateMgr()->PopState();
		break;
	case MenuPosition::ToTitle:
		gameMgr->Reinitialise();
		gameMgr->GetGameStateMgr()->ClearStates();
		GameConstants::GameIsReady = false;
		gameMgr->GetGameStateMgr()->ChangeState(new TitleState(gameMgr));
		break;
	case MenuPosition::Quit:
		gameMgr->GetRenderWindow().close();
		break;
	}
}

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: GameState("PauseMenu"),
	m_menu(&PauseMenuActionFunc,
		"Resume",
		30,
		20,
		sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f))
{
	m_menu.AddMenuItem("Return To Title Screen");
	m_menu.AddMenuItem("Quit");

	m_gameMgr = gameMgr;
}

void PauseMenuState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{
}

void PauseMenuState::ProcessInputs()
{
	m_menu.ProcessInput();
}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void PauseMenuState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_menu.Render(window);
}
