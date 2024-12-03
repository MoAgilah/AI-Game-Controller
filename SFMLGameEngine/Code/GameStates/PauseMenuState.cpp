#include "PauseMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/TitleState.h"

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: GameState("PauseMenu")
{
	m_gameMgr = gameMgr;
}

void PauseMenuState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));

	m_resumeMessage.Init("Resume", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f));

	m_toTitleMessage.Init("Return To Title Screen", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, (GameConstants::ScreenDim.y / 2.0f) + 50.f));
	m_toTitleMessage.Pause();

	m_quitMessage.Init("Quit", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, (GameConstants::ScreenDim.y / 2.0f) + 100.f));
	m_quitMessage.Pause();
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{
}

void PauseMenuState::ProcessInputs()
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
		if (m_menuPosition == MenuPosition::Resume)
		{
			m_gameMgr->GetGameStateMgr()->PopState();
		}
		else if (m_menuPosition == MenuPosition::ToTitle)
		{
			m_gameMgr->Reinitialise();
			m_gameMgr->GetGameStateMgr()->ClearStates();
			GameConstants::GameIsReady = false;
			m_gameMgr->GetGameStateMgr()->ChangeState(new TitleState(m_gameMgr));
		}
		else if (m_menuPosition == MenuPosition::Quit)
		{
			m_gameMgr->GetRenderWindow().close();
		}
	}
}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	if (m_menuMoved)
	{
		if (m_menuPosition < 0)
		{
			m_menuPosition = MenuPosition::Resume;
		}

		if (m_menuPosition >= m_maxMenuPosition)
		{
			m_menuPosition = m_maxMenuPosition;
		}

		switch (m_menuPosition)
		{
		case MenuPosition::Resume:
			m_resumeMessage.Resume();
			m_toTitleMessage.Pause();
			m_quitMessage.Pause();
			break;
		case MenuPosition::ToTitle:
			m_toTitleMessage.Resume();
			m_resumeMessage.Pause();
			m_quitMessage.Pause();
			break;
		case MenuPosition::Quit:
			m_quitMessage.Resume();
			m_resumeMessage.Pause();
			m_toTitleMessage.Pause();
			break;
		}

		m_menuMoved = false;
	}

	m_resumeMessage.Update(deltaTime);
	m_toTitleMessage.Update(deltaTime);
	m_quitMessage.Update(deltaTime);
}

void PauseMenuState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_resumeMessage.Render(window);
	m_toTitleMessage.Render(window);
	m_quitMessage.Render(window);
}
