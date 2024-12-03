#include "MainMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/MainState.h"

MainMenuState::MainMenuState(GameManager* gameMgr)
	: GameState("MainMenu")
{
	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));

	m_automationMessage.Init("Start Simulation", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f));
	m_manualMessage.Init("Play Manually", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, (GameConstants::ScreenDim.y / 2.0f) + 50.f));
	m_manualMessage.Pause();
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
	if (m_gameMgr->GetInputManager().IsKeyPressed(sf::Keyboard::Up))
	{
		if (!m_menuMoved)
		{
			m_menuPosition--;
			m_menuMoved = true;
		}
	}

	if (m_gameMgr->GetInputManager().IsKeyPressed(sf::Keyboard::Down))
	{
		if (!m_menuMoved)
		{
			m_menuPosition++;
			m_menuMoved = true;
		}
	}

	if (m_gameMgr->GetInputManager().IsKeyPressed(sf::Keyboard::Enter))
	{
		if (m_menuPosition == MenuPosition::Automation)
		{
			GameConstants::Automated = true;
		}
		else if (m_menuPosition == MenuPosition::Manual)
		{
			GameConstants::Automated = false;
		}

		m_gameMgr->GetGameStateMgr()->ChangeState(new MainState(m_gameMgr));
	}
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	if (m_menuMoved)
	{
		if (m_menuPosition < 0)
		{
			m_menuPosition = MenuPosition::Automation;
		}

		if (m_menuPosition >= m_maxMenuPosition)
		{
			m_menuPosition = m_maxMenuPosition;
		}

		switch (m_menuPosition)
		{
		case MenuPosition::Automation:
			m_automationMessage.Resume();
			m_manualMessage.Pause();
			break;
		case MenuPosition::Manual:
			m_manualMessage.Resume();
			m_automationMessage.Pause();
			break;
		}

		m_menuMoved = false;
	}

	m_automationMessage.Update(deltaTime);
	m_manualMessage.Update(deltaTime);
}

void MainMenuState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_automationMessage.Render(window);
	m_manualMessage.Render(window);
}
