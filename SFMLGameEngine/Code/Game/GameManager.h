#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Timer.h"
#include "Camera.h"
#include "InputManager.h"
#include "Logger.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "World.h"
#include "../GameObjects/Player.h"
#include "../Collisions/CollisionManager.h"
#include "../Controller/AIController.h"
#include "../GameStates/GameStateMgr.h"

class GameManager
{
public:
	GameManager(sf::RenderWindow& window);
	~GameManager();
	GameManager(const GameManager& obj) = delete;

	void Reinitialise();

	static GameManager* Get() { return m_instance; }

	void InitPlayer();

	void CheckInView();

	void Update(float deltaTime);
	void Render();

	sf::RenderWindow& GetRenderWindow() { return m_window; }

	void ChangePlayer(Player* ply);
	Player* GetPlayer() { return m_player; }

	void ChangeWorld(World* world);
	World* GetWorld() { return m_world.get(); }

	Timer& GetTimer() { return m_timer; }
	Camera& GetCamera() { return m_camera; }
	InputManager& GetInputManager() { return m_inputManager; }
	Logger& GetLogger() { return m_logger; }
	FontManager& GetFontMgr() { return m_fontManager; }
	TextureManager& GetTextureMgr() { return m_texureManager; }
	CollisionManager* GetCollisionMgr() { return m_collisionManager.get(); }
	AIController* GetAIController() { return m_aiController.get(); }
	GameStateMgr* GetGameStateMgr() { return &m_stateManager; }

private:

	static GameManager*	m_instance;
	sf::RenderWindow&					m_window;
	Timer								m_timer;
	Camera								m_camera;
	InputManager						m_inputManager;
	Logger								m_logger;
	FontManager							m_fontManager;
	TextureManager						m_texureManager;
	GameStateMgr						m_stateManager;
	Player*								m_player;
	std::unique_ptr<World>				m_world;
	std::unique_ptr<CollisionManager>	m_collisionManager;
	std::unique_ptr<AIController>		m_aiController;
};