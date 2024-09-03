#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include "Timer.h"
#include "World.h"
#include "../Collisions/CollisionManager.h"
#include "../GameStates/GameStateMgr.h"

class Player;
class GameManager
{
public:
	GameManager();
	~GameManager() = default;

	static GameManager* GetGameMgr() { return m_instance.get(); }

	Camera* GetCamera() { return m_camera.get(); }
	Player* GetPlayer() { return m_player.get(); }
	World* GetLevel() { return m_world.get(); }
	Logger* GetLogger() { return m_logger.get(); }
	Timer* GetTimer() { return m_timer.get(); }
	FontManager* GetFontMgr() { return m_fontManager.get(); }
	TextureManager* GetTextureMgr() { return m_texureManager.get(); }
	CollisionManager* GetCollisionMgr() { return m_collisionManager.get(); }

	void ChangePlayer(Player* ply);

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void CheckInView();

private:

	static std::shared_ptr<GameManager>	m_instance;
	std::unique_ptr<FontManager>		m_fontManager;
	std::unique_ptr<TextureManager>		m_texureManager;
	std::unique_ptr<Camera>				m_camera;
	std::unique_ptr<Player>				m_player;
	std::unique_ptr<World>				m_world;
	std::unique_ptr<CollisionManager>	m_collisionManager;
	std::unique_ptr<Logger>				m_logger;
	std::unique_ptr<Timer>				m_timer;
	GameStateMgr m_stateMgr;
};