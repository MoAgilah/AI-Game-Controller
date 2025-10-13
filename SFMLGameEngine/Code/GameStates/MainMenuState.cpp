#include "MainMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/MainState.h"

enum MenuPosition { Automation, Manual };

void MainMenuActionFunc(int menuPosition)
{
	switch (menuPosition)
	{
	case MenuPosition::Automation:
		GameConstants::Automated = true;
		break;
	case MenuPosition::Manual:
		GameConstants::Automated = false;
		break;
	}

	GameConstants::DControl = GameConstants::Automated;

#if defined(_DEBUG) || defined(DEBUG)
	GameConstants::DRender = true;
#else
	GameConstants::DRender = false;
#endif

	GameManager::Get()->GetGameStateMgr()->ChangeState(new MainState(GameManager::Get()));
}

MainMenuState::MainMenuState(GameManager* gameMgr)
	: GameState("MainMenu"),
	m_menu(&MainMenuActionFunc,
		"Start Simulation",
		30,
		20,
		sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f))
{
	m_menu.AddMenuItem("Play Manually");

	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
	m_menu.ProcessInput();
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_menu.Render(window);
}
