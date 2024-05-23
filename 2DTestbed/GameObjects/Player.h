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
	Player(std::string filepath, int rows, int cols, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Player();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	BoundingBox* GetBBox();

	void Move(sf::Vector2f vel);
	sf::Vector2f GetPosition() const;
	void SetPosition(sf::Vector2f);
	void SetPosition(float x, float y);

	sf::Vector2f GetOrigin() const;

	sf::Vector2f GetPrevPostion();
	void SetPrevPosition(sf::Vector2f);
	void SetPrevPosition(float x, float y);

	void SetSpawnLoc(sf::Vector2f loc = sf::Vector2f(0, 0));
	void CheckPointHit(bool hit);

	bool GetIsSuper();
	void SetIsSuper(bool super);

	void IncreaseCoins(int num);

	void Reset();

	bool GetGoalHit();
	void GoalHit();
	bool GetIsAlive();
	void JustBeenHit(bool hit);
	bool GetIfInvulnerable();
	void Kill();
	void ReSpawn();

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

	AnimatedSprite * m_currSpr;
	BoundingBox * m_curBbox;

	AnimatedSprite * m_SupSpr;
	BoundingBox * m_SupBbox;

	BoundingBox * m_CrouchBbox;
	BoundingBox * m_SCrouchBbox;

	int m_coinTotal;

	bool hitChkPnt;
	bool ifWasSuper;
	bool m_spindown;
	bool m_super;
	bool justCrouched;
	bool m_prevDirection;
	bool justBeenHit;
	bool die;
	bool killed;
	bool m_goingUp;
	bool m_alive;
	bool cantjump;
	bool cantSpinJump;
	bool justHitEnemy;
	bool goalHit;
	bool m_keyState[MAXKEYS] = { false };

	float heightDiff;
	float m_noGravTime;
	float m_InvulTime;
	float m_airtime;
	const float c_maxAirTime = .9f;
	const float m_moveSpeed = 3.0f;
	const float m_jumpSpeed = 3.0f;
};

#endif
