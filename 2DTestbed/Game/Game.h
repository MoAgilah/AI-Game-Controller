#pragma once

#include <memory>
#include <SFML\Graphics.hpp>

#include "Camera.h"
#include "World.h"
#include "Logger.h"
#include "Timer.h"
#include "../Drawables/TextureManager.h"

class Player;
class Game
{
public:
	Game();
	~Game() = default;

	static Game* GetGameMgr() { return m_instance.get(); }

	Camera* GetCamera() { return m_camera.get(); }
	Player* GetPlayer() { return m_player.get(); }
	World* GetLevel() { return m_world.get(); }
	Logger* GetLogger() { return m_logger.get(); }

	void ChangePlayer(Player* ply);

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
private:
	void CheckInView();

	static std::unique_ptr<Game>	m_instance;
	std::unique_ptr<TextureManager>	m_texureManager;
	std::unique_ptr<Camera>			m_camera;
	std::unique_ptr<Player>			m_player;
	std::unique_ptr<World>			m_world;
	std::unique_ptr<Logger>			m_logger;
};