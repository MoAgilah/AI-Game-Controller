#pragma once

#include <vector>
#include "GameObjectState.h"

class GameObjectStateMgr
{
public:
	~GameObjectStateMgr();

	std::string_view GetStateName();

	void ChangeState(GameObjectState* state);
	void PushState(GameObjectState* state);
	void PopState();
	void ClearStates();

	void Pause();
	void Resume();

	void ProcessInputs();
	void Update(float deltaTime);

private:
	std::vector<GameObjectState*> m_vGameStates;
};
