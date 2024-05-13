#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "../Utils.h"
#include "Logger.h"

#include "Level.h"
#include "Timer.h"

class Player;
class Game
{
public:
	~Game();
	static Game* GetGameMgr();
	void ChangePlayer(Player* ply);
	Player* GetPlayer();
	Level* GetLevel();
	Logger* GetLogger();
	void CheckInView();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
private:
	Game();

	static Game* m_instance;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Level> m_level;
	std::unique_ptr<Logger> m_logger;
};