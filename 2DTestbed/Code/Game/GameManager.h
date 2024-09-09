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
	~GameManager();
	GameManager(const GameManager& obj) = delete;

	static GameManager* GetGameMgr() { return m_instance; }

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void ChangePlayer(Player* ply);
	Player* GetPlayer() { return m_player.get(); }

	void ChangeWorld(World* world);
	World* GetWorld() { return m_world.get(); }

	void CheckInView();

	CollisionManager* GetCollisionMgr() { return m_collisionManager.get(); }

	Timer& GetTimer() { return m_timer; }
	Camera& GetCamera() { return m_camera; }
	Logger& GetLogger() { return m_logger; }
	FontManager& GetFontMgr() { return m_fontManager; }
	TextureManager& GetTextureMgr() { return m_texureManager; }

private:

	static GameManager*	m_instance;


	std::unique_ptr<CollisionManager>	m_collisionManager;

	Timer						m_timer;
	Camera						m_camera;
	Logger						m_logger;
	FontManager					m_fontManager;
	TextureManager				m_texureManager;
	GameStateMgr				m_stateManager;
	std::unique_ptr<Player>		m_player;
	std::unique_ptr<World>		m_world;
};