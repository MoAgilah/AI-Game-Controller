#include "GameStateMgr.h"
#include <iostream>
#include <format>
#include "../Game/Constants.h"

GameStateMgr::~GameStateMgr()
{
	ClearStates();
}

std::string_view GameStateMgr::GetStateName()
{
	if (!m_vGameStates.empty())
		return m_vGameStates.back()->GetStateName();

	return std::string_view();
}

void GameStateMgr::ChangeState(GameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.pop_back();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();

	if (GameConstants::DRender)
		std::cout << std::format("Changed too {} state\n", m_vGameStates.back()->GetStateName());
}

void GameStateMgr::PushState(GameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();

	if (GameConstants::DRender)
		std::cout << std::format("Pushed {} state\n", m_vGameStates.back()->GetStateName());
}

void GameStateMgr::PopState()
{
	if (!m_vGameStates.empty())
	{
		if (GameConstants::DRender)
			std::cout << std::format("Popped {} state\n", m_vGameStates.back()->GetStateName());
		m_vGameStates.pop_back();
	}

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameStateMgr::ClearStates()
{
	while (!m_vGameStates.empty())
		m_vGameStates.pop_back();
}

void GameStateMgr::Pause()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();
}

void GameStateMgr::Resume()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameStateMgr::ProcessInputs()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->ProcessInputs();
}

void GameStateMgr::Update(float deltaTime)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Update(deltaTime);
}

void GameStateMgr::Render(sf::RenderWindow& window)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Render(window);
}
