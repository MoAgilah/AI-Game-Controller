#pragma once

#include <memory>
#include "GameObjectState.h"
#include "../GameObjects/Player.h"

class PlayerState : public GameObjectState
{
public:
	explicit PlayerState(std::string_view name, Player* ply)
		: GameObjectState(name)
	{
		m_player.reset(ply);
	}

	~PlayerState() override = default;

	void Pause() override { m_player->SetActive(false); }
	void Resume() override { m_player->SetActive(true); }

	Player* GetPlayer() { return m_player.get(); }

private:
	std::shared_ptr<Player> m_player;
};

class GroundedState : public PlayerState
{
public:
	explicit GroundedState(Player* ply)
		: PlayerState("Grounded", ply) {}
	~GroundedState() override = default;

	void Initialise() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};

class AirborneState : public PlayerState
{
public:
	explicit AirborneState(Player* ply)
		: PlayerState("Airborne", ply) {}
	~AirborneState() override = default;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};

class CrouchingState : public PlayerState
{
public:
	explicit CrouchingState(Player* ply)
		: PlayerState("Crouching", ply) {}
	~CrouchingState() override = default;

	void Initialise() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};

class DieingState : public PlayerState
{
public:
	explicit DieingState(Player* ply)
		: PlayerState("Dieing", ply) {}
	~DieingState() override = default;

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};