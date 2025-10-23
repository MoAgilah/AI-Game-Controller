#pragma once

#include <memory>
#include "GameState.h"
#include "../Game/Timer.h"
#include "../GameObjects/Player.h"

class PlayerState : public GameState
{
public:
	explicit PlayerState(std::string_view name, Player* ply)
		: GameState(name)
	{
		m_player = ply;
		m_animSpr = m_player->GetAnimSpr();
		m_physCtrl = m_player->GetPhysicsController();
	}

	~PlayerState() override
	{
		if (m_player)
			m_player = nullptr;

		if (m_animSpr)
			m_animSpr = nullptr;

		if (m_physCtrl)
			m_physCtrl = nullptr;
	}

	void Pause() override { m_player->SetActive(false); }
	void Resume() override { m_player->SetActive(true); }

	Player* GetPlayer() { return m_player; }

	static float s_frameStep;

protected:

	Player* m_player;
	AnimatedSprite* m_animSpr;
	PhysicsController* m_physCtrl;
};

class GroundedState : public PlayerState
{
public:
	explicit GroundedState(Player* ply)
		: PlayerState("Grounded", ply), m_turnTimer(0) {}

	void Initialise() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;

private:

	void UpdateGroundAnimation();
	void Slide(bool dir);

	Timer m_turnTimer;
	bool m_turningAround = false;
};

class AirborneState : public PlayerState
{
public:
	explicit AirborneState(Player* ply)
		: PlayerState("Airborne", ply) {}

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};

class CrouchingState : public PlayerState
{
public:
	explicit CrouchingState(Player* ply)
		: PlayerState("Crouching", ply) {}

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

	void Initialise() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
};