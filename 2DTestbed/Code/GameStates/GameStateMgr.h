#pragma once

#include <vector>
#include "GameState.h"

class GameStateMgr
{
public:
	~GameStateMgr();

	std::string_view GetStateName();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();
	void ClearStates();

	void Pause();
	void Resume();

	void ProcessInputs();
	void Update(float deltaTime);

private:
	std::vector<GameState*> m_vGameStates;
};
