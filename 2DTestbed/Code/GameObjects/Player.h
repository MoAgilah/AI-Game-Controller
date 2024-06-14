#pragma once

#include <array>
#include <string>
#include "../Game/Constants.h"
#include "../GameObjects/GameObject.h"
#include "../NEAT/phenotype.h"

#include "../GameObjectState/GameObjectStateMgr.h"

enum Anims { IDLE, CROUCH, JUMP, MOVING, LOOKUP, SPINJUMP, FALL, DIE, MAXANIM };

enum Actions { LEFT_KEY, RIGHT_KEY, UP_KEY, DOWN_KEY, JUMP_KEY, RCRTL_KEY };

class Camera;
class Player : public AnimatedGameObject
{
public:
	Player();
	~Player() final = default;

	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;
	void Reset() final;

	void Move(sf::Vector2f vel);

	bool GetIsSuper() const { return m_super; }
	void SetIsSuper(bool super);

	bool GetGoalHit() const { return m_goalHit; }
	void GoalHit() { m_goalHit = true; }

	void UpdateFitness(float fitness) { m_fitness += fitness; }
	double Fitness() const { return m_fitness; }

	bool GetIsAlive() const { return m_alive; }
	void SetIsAlive(bool val) { m_alive = val; }

	void IncreaseCoins(int num) { m_coinTotal = +num; }

	const std::array<bool, MAXKEYS>& GetKeyStates() const { return m_keyStates; }

	bool GetIfInvulnerable() const { return m_justBeenHit; }

	void SetSpawnLoc(sf::Vector2f loc = sf::Vector2f(0, 0));

	bool GetCantJump() const { return m_cantjump; }
	void SetCantJump(bool val) { m_cantjump = val; }

	bool GetCantSpinJump() const { return m_cantSpinJump; }
	void SetCantSpinJump(bool val) { m_cantSpinJump = val; }

	bool GetIsCrouched() const { return m_crouched; }
	void SetIsCrouched(bool crouched) { m_crouched = crouched; }

	void ForceFall();

	void JustBeenHit(bool hit);
	void JusyHitEnemy(float val = 1);

	float GetAirTime() const { return m_airtime; }
	void SetAirTime(float val) { m_airtime = val; }
	void IncAirTime(float val) { m_airtime += val; }

	void InsertNewBrain(CNeuralNet* brain) { m_itsBrain = brain; }
	bool UpdateANN();

private:
	void ProcessInput();
	void ControllerInput();
	void HumanInput();

	static bool s_playerInserted;
	bool m_super = false;
	bool m_crouched = false;
	bool m_justBeenHit = false;
	bool m_justHitEnemy = false;
	bool m_alive = true;
	bool m_cantjump = false;
	bool m_cantSpinJump = false;
	bool m_goalHit = false;

	std::array<bool, MAXKEYS> m_keyStates;

	int m_coinTotal = 0;

	sf::Vector2f m_sizeDiff;
	float m_heightDiff = 11.25;
	float m_noGravTime = 0;
	float m_InvulTime = 0;
	float m_airtime = 0;

	double	m_fitness = 0;
	std::vector<double> outputs;

	sf::Vector2f m_spawnLoc;

	CNeuralNet* m_itsBrain = nullptr;

	sf::Shader m_fragShader;
	GameObjectStateMgr m_stateMgr;
};
