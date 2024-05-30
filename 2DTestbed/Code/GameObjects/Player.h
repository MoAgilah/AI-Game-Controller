#ifndef PlayerH
#define PlayerH

#include <string>
#include "../GameObjects/GameObject.h"
#include "../NEAT/phenotype.h"

enum Anims { IDLE, CROUCH, JUMP, LEFT, RIGHT = LEFT, LOOKUP, SPINJUMP, FALL, DIE, MAXANIM };

enum Actions { LEFT_KEY, RIGHT_KEY, UP_KEY, DOWN_KEY, SPACE_KEY, RCRTL_KEY, MAXKEYS };

class Camera;
class Player : public GameObject
{
public:
	Player(int rows, int cols, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Player() final;

	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;

	void Move(sf::Vector2f vel);

	void SetSpawnLoc(sf::Vector2f loc = sf::Vector2f(0, 0));

	bool GetIsSuper();
	void SetIsSuper(bool super);

	void IncreaseCoins(int num);

	void Reset() final;

	bool GetGoalHit();
	void GoalHit();
	bool GetIsAlive();
	void JustBeenHit(bool hit);
	bool GetIfInvulnerable();
	void Kill();

	void SetAirTime(float val = 1);
	void SetCantJump();

	void UpdateFitness(float fitness);
	void EndOfRunCalculations();
	void InsertNewBrain(CNeuralNet* brain) { m_pItsBrain = brain; }
	bool UpdateANN();
	double Fitness() const;
private:
	//Controller code
	static bool s_playerInserted;
	CNeuralNet*  m_pItsBrain;
	//the players fitness score.
	double			m_dFitness;
	std::vector<double> outputs;
	void ControllerInput();
	//Controller code

	void HumanInput();
	void ProcessInput();

	sf::Vector2f m_spawnLoc;
	sf::Vector2f m_deathLoc;

	AnimatedSprite * m_SupSpr;
	BoundingBox * m_SupBbox;

	BoundingBox * m_CrouchBbox;
	BoundingBox * m_SCrouchBbox;

	int m_coinTotal;

	bool m_super = false;
	bool m_justCrouched = false;
	bool m_justBeenHit = false;
	bool m_justHitEnemy = false;
	bool m_alive = true;
	bool m_cantjump = false;
	bool m_cantSpinJump = false;
	bool m_goalHit = false;
	bool m_keyState[MAXKEYS] = { false };

	float m_heightDiff = 0;
	float m_noGravTime = 0;
	float m_InvulTime = 0;
	float m_airtime = 0;
	const float c_maxAirTime = .9f;
	const float m_moveSpeed = 3.0f;
	const float m_jumpSpeed = 3.0f;
};

#endif
