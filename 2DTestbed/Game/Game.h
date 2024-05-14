#ifndef GameH
#define GameH

#include <SFML\Graphics.hpp>
#include "../Utils.h"


#include "Level.h"
#include "Timer.h"
#include "Logger.h"

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
	Player* m_player;
	Level* m_level;
	Logger* m_logger;
};



#endif