#include "GameObjectStateMgr.h"

GameObjectStateMgr::~GameObjectStateMgr()
{
	while (!m_vGameStates.empty())
		m_vGameStates.pop_back();
}

std::string_view GameObjectStateMgr::GetStateName()
{
	if (!m_vGameStates.empty())
		return m_vGameStates.back()->GetStateName();

	return std::string_view();
}

void GameObjectStateMgr::ChangeState(GameObjectState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.pop_back();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void GameObjectStateMgr::PushState(GameObjectState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();
}

void GameObjectStateMgr::PopState()
{
	if (!m_vGameStates.empty())
		m_vGameStates.pop_back();

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameObjectStateMgr::Pause()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();
}

void GameObjectStateMgr::Resume()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void GameObjectStateMgr::ProcessInputs()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->ProcessInputs();
}

void GameObjectStateMgr::Update(float deltaTime)
{
	ProcessInputs();

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Update(deltaTime);
}
