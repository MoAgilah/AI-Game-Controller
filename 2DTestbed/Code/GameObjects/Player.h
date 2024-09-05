#pragma once

#include <array>
#include <string>
#include "../NEAT/phenotype.h"
#include "../Game/Constants.h"
#include "../Game/Timer.h"
#include "../GameObjects/Object.h"
#include "../GameStates/GameStateMgr.h"

enum MarioAnims { IDLE, LOOKUP, JUMP, FALL, RUNJUMP, SKID, SLIDE, VICTORY, CROUCH, GROW, DIE, MOVING, RUNNING, SPINJUMP };

enum MarioBoxes { REGULAR, SUPER, CROUCHED, NUMBOXES };

enum Keys { LEFT_KEY, RIGHT_KEY, UP_KEY, DOWN_KEY, RUN_KEY, JUMP_KEY, SJUMP_KEY, MAXKEYS };

class Player : public DynamicObject
{
public:
	explicit Player(const sf::Vector2f& pos);
	~Player() override = default;

	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;
	void Reset() final;

	bool GetIsSuper() const { return m_super; }
	void SetIsSuper(bool super);

	bool GetGoalHit() const { return m_goalHit; }
	void SetGoalHit(bool goal) { m_goalHit = goal; }

	bool GetIsAlive() const { return m_alive; }
	void SetIsAlive(bool val, float airtime = 0.1f);

	void IncreaseCoins(int num) { m_coinTotal = +num; }

	const std::array<bool, Keys::MAXKEYS>& GetKeyStates() const { return m_keyStates; }
	void SetKeyState(int index, bool val);

	void SetSpawnLoc(sf::Vector2f loc = sf::Vector2f(0, 0));

	bool GetCantJump() const { return m_cantjump; }
	void SetCantJump(bool val) { m_cantjump = val; }

	bool GetCantSpinJump() const { return m_cantSpinJump; }
	void SetCantSpinJump(bool val) { m_cantSpinJump = val; }

	bool GetIsCrouched() const { return m_crouched; }
	void SetIsCrouched(bool crouched);

	void ForceFall();

	bool GetIfInvulnerable() const { return !m_invulTimer.CheckEnd(); }
	void SetInvulnerability();

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air);

	Timer* GetAirTimer() { return &m_airTimer; }

private:

	void ProcessInput();
	virtual void Input();

	bool m_super = false;
	bool m_crouched = false;
	bool m_alive = true;
	bool m_cantjump = false;
	bool m_cantSpinJump = false;
	bool m_goalHit = false;
	bool m_airbourne = false;
	int m_coinTotal = 0;
	float m_heightDiff = 11.25;
	Timer m_airTimer;
	Timer m_invulTimer;
	sf::Vector2f m_spawnLoc;
	sf::Shader m_fragShader;
	GameStateMgr m_stateMgr;
	std::array<bool, Keys::MAXKEYS> m_keyStates;
	std::array<sf::Vector2f, MarioBoxes::NUMBOXES> m_boxSizes{ sf::Vector2f(9,16), sf::Vector2f(9,25), sf::Vector2f(14,11) };
};

class AutomatedPlayer : public Player
{
public:
	AutomatedPlayer(const sf::Vector2f& pos);
	~AutomatedPlayer() final = default;

	void UpdateFitness(float fitness) { m_fitness += fitness; }
	double Fitness() const { return m_fitness; }

	void InsertNewBrain(CNeuralNet* brain) { m_itsBrain = brain; }
	bool UpdateANN();

private:

	void Input() final;

	static bool s_playerInserted;
	double	m_fitness = 0;
	std::vector<double> outputs;
	CNeuralNet* m_itsBrain = nullptr;
};