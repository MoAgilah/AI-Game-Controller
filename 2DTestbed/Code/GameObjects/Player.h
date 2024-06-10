#pragma once

#include <array>
#include <string>
#include "../Game/Constants.h"
#include "../GameObjects/GameObject.h"
#include "../NEAT/phenotype.h"

enum Anims { IDLE, CROUCH, JUMP, LEFT, RIGHT = LEFT, LOOKUP, SPINJUMP, FALL, DIE, MAXANIM };

enum Actions { LEFT_KEY, RIGHT_KEY, UP_KEY, DOWN_KEY, SPACE_KEY, RCRTL_KEY };

class Camera;
class Player : public AnimatedGameObject
{
public:
	Player(int rows, int cols, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Player() final = default;

	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;

	void Reset() final;

	void Kill();

	void Move(sf::Vector2f vel);

	bool GetIsSuper() const { return m_super; }
	void SetIsSuper(bool super) { m_super = super; }

	bool GetGoalHit() const { return m_goalHit; }
	void GoalHit() { m_goalHit = true; }

	void UpdateFitness(float fitness) { m_fitness += fitness; }
	double Fitness() const { return m_fitness; }

	bool GetIsAlive() const { return m_alive; }

	void IncreaseCoins(int num) { m_coinTotal = +num; }

	bool GetIfInvulnerable() const { return m_justBeenHit; }

	void SetSpawnLoc(sf::Vector2f loc = sf::Vector2f(0, 0));

	void SetCantJump();

	void JustBeenHit(bool hit);

	void SetAirTime(float val = 1);

	void EndOfRunCalculations();
	void InsertNewBrain(CNeuralNet* brain) { m_itsBrain = brain; }
	bool UpdateANN();

private:
	void ProcessInput();
	void ControllerInput();
	void HumanInput();

private:
	static bool s_playerInserted;
	bool m_super = false;
	bool m_justCrouched = false;
	bool m_justBeenHit = false;
	bool m_justHitEnemy = false;
	bool m_alive = true;
	bool m_cantjump = false;
	bool m_cantSpinJump = false;
	bool m_goalHit = false;

	std::array<bool, MAXKEYS> m_keyStates;

	int m_coinTotal = 0;

	float m_heightDiff = 0;
	float m_noGravTime = 0;
	float m_InvulTime = 0;
	float m_airtime = 0;

	double	m_fitness;
	std::vector<double> outputs;

	sf::Vector2f m_spawnLoc;

	CNeuralNet* m_itsBrain;
};
