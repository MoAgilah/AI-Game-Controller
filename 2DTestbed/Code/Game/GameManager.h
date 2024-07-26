#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "World.h"
#include "Logger.h"
#include "Timer.h"
#include "TextureManager.h"

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
	TextureManager* GetTexturMgr() { return m_texureManager.get(); }
	CollisionManager* GetCollisionMgr() { return m_collisionManager.get(); }

	void ChangePlayer(Player* ply);

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void CheckInView();

	bool IsBoxObject(TexID id);
	bool IsEnemyObject(TexID id);
	bool IsPlayerObject(TexID id);
	bool IsCollectableObject(TexID id);
	bool IsDynamicObject(TexID id, int bgn = 0, int end = 7);
private:

	std::array<TexID, 5> m_enemyObjIds;
	std::array<TexID, 7> m_dynamicObjIds;
	std::array<TexID, 5> m_collectableIds;
	static std::shared_ptr<GameManager>		m_instance;
	std::unique_ptr<TextureManager>		m_texureManager;
	std::unique_ptr<Camera>				m_camera;
	std::unique_ptr<Player>				m_player;
	std::unique_ptr<World>				m_world;
	std::unique_ptr<CollisionManager>	m_collisionManager;
	std::unique_ptr<Logger>				m_logger;
	GameStateMgr m_stateMgr;
};