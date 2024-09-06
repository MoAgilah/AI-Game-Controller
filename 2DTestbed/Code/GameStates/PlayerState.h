#pragma once

#include <memory>
#include "GameState.h"
#include "../GameObjects/Player.h"

class PlayerState : public GameState
{
public:
	explicit PlayerState(std::string_view name, Player* ply)
		: GameState(name)
	{
		m_player.reset(ply);
		m_animSpr.reset(m_player->GetAnimSpr());
		m_physCtrl.reset(m_player->GetPhysicsController());
	}

	~PlayerState() override = default;

	void Pause() override { m_player->SetActive(false); }
	void Resume() override { m_player->SetActive(true); }

	Player* GetPlayer() { return m_player.get(); }

protected:

	std::shared_ptr<Player> m_player;
	std::shared_ptr<AnimatedSprite> m_animSpr;
	std::shared_ptr<PhysicsController> m_physCtrl;
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

private:

	void UpdateGroundAnimation();
	void Slide(bool dir);

	float m_turnTime = 0;
	bool m_turningAround = false;
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