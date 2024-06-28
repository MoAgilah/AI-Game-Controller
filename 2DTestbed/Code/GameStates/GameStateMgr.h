#pragma once

#include <vector>
#include "GameState.h"
#include <SFML/Graphics.hpp>

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
	void Render(sf::RenderWindow& window);

private:
	std::vector<GameState*> m_vGameStates;
};
