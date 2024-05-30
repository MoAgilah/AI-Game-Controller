#include "../GameObjects/Player.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"
#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../Controller/CtrlMgr.h"

bool Player::s_playerInserted = false;

Player::Player(int rows, int cols, bool symmetrical, int initAnim, float animSpd)
	: GameObject(TexID::Mario, rows, cols, (int)TexID::MarioBB, true, symmetrical, initAnim, animSpd)
{
	m_type = PLAYER;
	m_spawnData.m_initialPos = sf::Vector2f(75, 454);

	std::vector<int> frames{ 1, 1, 1, 2, 1, 2, 1, 2 };
	//regular mario
	m_curSpr->SetFrames(frames);
	auto tmp = Collisions::Get()->GetTile(151, 5);

	SetPosition(m_spawnData.m_initialPos);
	m_CrouchBbox = new BoundingBox(TexID::MarioSmlBB);

	//super mario
	frames = std::vector<int>{ 1, 1, 1, 3, 1, 2, 1, 2 };
	m_SupSpr = new AnimatedSprite(TexID::Super, rows, cols + 1, FPS, symmetrical, initAnim, animSpd);
	m_SupSpr->SetFrames(frames);

	m_SupBbox = new BoundingBox(TexID::SuperBB);
	m_SCrouchBbox = new BoundingBox(TexID::SuperSmlBB);

	m_heightDiff = m_SupSpr->GetOrigin().y * sY - m_spr->GetOrigin().y * sY;

	//if automated
	if (Automated)
	{
		//if player already inserted
		if (s_playerInserted)
		{
			//remove additional players
			Collisions::Get()->RemoveLastAdded();
		}
		else
		{
			//if first player
			s_playerInserted = true;
		}
	}
	else
	{
		s_playerInserted = true;
	}
}

Player::~Player()
{
	if (m_SupSpr)
	{
		delete m_SupSpr;
		m_SupSpr = nullptr;
	}

	if (m_SupBbox)
	{
		delete m_SupBbox;
		m_SupBbox = nullptr;
	}

	if (m_SCrouchBbox)
	{
		delete m_SCrouchBbox;
		m_SCrouchBbox = nullptr;
	}

	if (m_CrouchBbox)
	{
		delete m_CrouchBbox;
		m_CrouchBbox = nullptr;
	}
}

void Player::Update(float deltaTime)
{
	if (!m_active)
		return;

	//adjust position when changing from regular -> super and vice versa
	if (m_super)
	{
		//if current spr and bbox is not super
		if (m_curSpr != m_SupSpr)
		{
			//change spr and bbox
			m_curSpr = m_SupSpr;
			m_curBBox = m_SupBbox;

			//adjust postion
			SetPosition(m_spr->GetPosition() - sf::Vector2f(0, m_heightDiff));
		}
	}
	else
	{
		//if current spr and bbox is not regular
		if (m_curSpr != m_spr.get())
		{
			//change spr and bbox
			m_curSpr = m_spr.get();
			m_curBBox = m_bbox.get();

			//adjust position
			SetPosition(m_SupSpr->GetPosition() + sf::Vector2f(0, m_heightDiff));
		}

	}//end super

	ProcessInput();

	//set vulnerability time
	if (m_justBeenHit)
	{
		m_InvulTime -= deltaTime;

		if (m_InvulTime <= 0)
		{
			m_justBeenHit = false;
		}
	}

	if (m_justHitEnemy)
	{
		//if not hovering
		if (m_noGravTime <= 0)
		{
			if (m_justHitEnemy)
				m_justHitEnemy = false;
		}
		else //if hovering
		{
			m_velocity.y = 0;
			m_noGravTime -= deltaTime;
		}
	}

	if (!GetIsAlive())
	{
		if (m_airbourne)
		{
			m_velocity.y = -m_jumpSpeed;
			m_airtime += deltaTime;
			if (m_airtime >= c_maxAirTime)
			{
				m_airbourne = false;
			}
		}
		else
		{
			m_velocity.y += gravity;
			if (!Game::GetGameMgr()->GetCamera()->OnScreen(this))
			{
				if (!Automated)
				{
					Reset();
				}
			}
		}

		Move(sf::Vector2f(0, m_velocity.y * FPS * deltaTime));
	}
	else
	{
		if (!GetOnGround())
		{
			if (m_airbourne)
			{
				m_airtime += deltaTime;
				if (m_airtime >= c_maxAirTime)
				{
					m_airbourne = false;
					m_justCrouched = false;
				}
			}
			else
			{
				m_velocity.y += gravity;
			}
		}
		else
		{
			m_velocity.y = 0;
			m_airtime = 0;
		}

		//decomposition of movement
		if (m_velocity.x != 0)
		{
			SetPrevPosition(m_curSpr->GetPosition());
			Move(sf::Vector2f(m_velocity.x * FPS * deltaTime, 0));
			Collisions::Get()->ProcessCollisions(this);
		}

		if (m_velocity.y != 0)
		{
			SetPrevPosition(m_curSpr->GetPosition());
			Move(sf::Vector2f(0, m_velocity.y * FPS * deltaTime));
			Collisions::Get()->ProcessCollisions(this);
		}
	}

	//check for leftmost and rightmost boundary
	if (m_curSpr->GetPosition().x < m_curSpr->GetOrigin().x || m_curSpr->GetPosition().x > 11776 - m_curSpr->GetOrigin().x)
	{
		Move(sf::Vector2f(-m_velocity.x * FPS * deltaTime, 0));
	}

	if (m_curBBox->GetSprite()->getPosition().y /*+ m_curBBox->GetSprite()->getOrigin().y*/ > 600 - m_curBBox->GetSprite()->getOrigin().y)
	{
		if (GetIsAlive())
			Kill();
	}

	//check for exceeded rightmost || or hit the goal
	if (m_curSpr->GetPosition().x > RightMost)
	{
		SetSpawnLoc();

		if (Automated == false)
		{
			Reset();
		}
	}

	m_curSpr->Update(deltaTime, m_direction);
}

void Player::Render(sf::RenderWindow& window)
{
	m_curSpr->Render(window);
	m_curBBox->Render(window);
}

void Player::Move(sf::Vector2f vel)
{
	m_curSpr->Move(vel.x, vel.y);
	m_curBBox->GetSprite()->move(vel);
}

bool Player::GetIsSuper()
{
	return m_super;
}

void Player::SetIsSuper(bool super)
{
	m_super = super;
}

void Player::SetSpawnLoc(sf::Vector2f loc)
{
	if (loc == sf::Vector2f(0, 0))
	{
		m_spawnLoc = m_spawnData.m_initialPos;
	}
	else
	{
		m_spawnLoc.x = loc.x;
	}
}

void Player::IncreaseCoins(int num)
{
	m_coinTotal = +num;
	//m_dFitness += 100;
}

void Player::Reset()
{
	GameObject::Reset();

	m_spawnLoc = m_spawnData.m_initialPos;

	m_velocity = sf::Vector2f(0.0f, 0.0f);

	m_super = false;
	m_justCrouched = false;
	m_justBeenHit = false;
	m_alive = true;
	m_cantjump = false;
	m_cantSpinJump = false;
	m_cantSpinJump = false;
	m_goalHit = false;

	for (size_t i = 0; i < MAXKEYS; i++)
	{
		m_keyState[i] = false;
	}

	m_heightDiff = 0;
	m_noGravTime = 0;
	m_InvulTime = 0;
	m_airtime = 0;

	Timer::Get()->ResetTime();
	Game::GetGameMgr()->GetLevel()->ResetLevel();
}

bool Player::GetGoalHit()
{
	return m_goalHit;
}

void Player::SetAirTime(float val)
{
	m_cantSpinJump = true;
	m_noGravTime = val;
}

void Player::JustBeenHit(bool hit)
{
	m_justBeenHit = hit;
	m_InvulTime = 1;
}

bool Player::GetIfInvulnerable()
{
	return m_justBeenHit;
}

void Player::Kill()
{
	m_airtime = 0.33f;
	m_airbourne = true;
	m_onGround = false;
	m_alive = false;
	m_curSpr->ChangeAnim(DIE);//Die
}

void Player::GoalHit()
{
	m_goalHit = true;
}

bool Player::GetIsAlive()
{
	return m_alive;
}

void Player::SetCantJump()
{
	if (m_cantjump == false || m_cantSpinJump == false)
	{
		m_cantjump = true;
		m_cantSpinJump = true;

		if (m_airbourne)
		{
			m_airbourne = false;
		}
	}
}

void Player::HumanInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_keyState[LEFT_KEY] = true;
	}
	else
	{
		if (m_keyState[LEFT_KEY])
		{
			m_keyState[LEFT_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_keyState[RIGHT_KEY] = true;
	}
	else
	{
		if (m_keyState[RIGHT_KEY])
		{
			m_keyState[RIGHT_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_keyState[SPACE_KEY] = true;
	}
	else
	{
		if (m_keyState[SPACE_KEY])
		{
			m_keyState[SPACE_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		m_keyState[RCRTL_KEY] = true;
	}
	else
	{
		if (m_keyState[RCRTL_KEY])
		{
			m_keyState[RCRTL_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_keyState[UP_KEY] = true;
	}
	else
	{
		if (m_keyState[UP_KEY])
		{
			m_keyState[UP_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_keyState[DOWN_KEY] = true;
	}
	else
	{
		if (m_keyState[DOWN_KEY])
		{
			m_keyState[DOWN_KEY] = false;
		}
	}
}

bool Player::UpdateANN()
{
	std::vector<double> inputs;

	inputs = CtrlMgr::GetCtrlMgr()->GetController()->GetGridInputs();

	outputs = m_pItsBrain->Update(inputs, CNeuralNet::active);

	if (outputs.size() < CParams::iNumOutputs)
	{
		return false;
	}

	return true;
}

double Player::Fitness() const
{
	return m_dFitness;
}

void Player::UpdateFitness(float fitness)
{
	m_dFitness += fitness;
}

void Player::EndOfRunCalculations()
{
	float percent = ((GetPosition().x - 18.f) / RightMost) * 100;
	//completed level
	if (m_goalHit)
	{
		Game::GetGameMgr()->GetLogger()->AddExperimentLog(" Completed the level");
		m_dFitness += 1000;
	}
	else if (GetPosition().x <= 75.f)
	{
		m_dFitness -= percent;

		if (GetPosition().x == 75.f)
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog(" Did not move = " + std::to_string(percent) + "% Completed");
		}
		else
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog(" Moved left  = " + std::to_string(percent) + "% Completed");
		}
	}
	//moved right some
	else
	{
		Game::GetGameMgr()->GetLogger()->AddExperimentLog(" " + std::to_string(percent) + "% Completed");
		m_dFitness += percent;
	}
}

void Player::ControllerInput()
{
	Game::GetGameMgr()->GetLogger()->AddDebugLog("Player " + std::to_string(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayerNum()), false);

	for (int i = 0; i < outputs.size(); ++i)
	{
		bool output = false;
		double oval = outputs[i];

		std::string move = "";
		switch (i)
		{
		case LEFT_KEY:
			move = "left";
			break;
		case RIGHT_KEY:
			move = "rght";
			break;
		case UP_KEY:
			move = "look";
			break;
		case DOWN_KEY:
			move = "down";
			break;
		case SPACE_KEY:
			move = "jump";
			break;
		case RCRTL_KEY:
			move = "sJmp";
			break;

		default:
			break;
		}

		//clamp output
		if (oval <= 0.1) output = false;
		else if (oval >= 0.9) output = true;
		else output = false;

		Game::GetGameMgr()->GetLogger()->AddDebugLog(move + " = " + std::to_string(oval) + " = " + std::to_string(output), false);
		Game::GetGameMgr()->GetLogger()->AddDebugLog("\t", false);
		//store output
		m_keyState[i] = output;
	}
	Game::GetGameMgr()->GetLogger()->AddDebugLog("");
}

void Player::ProcessInput()
{
	if (!GetIsAlive())
		return;

	if (Automated)
	{
		ControllerInput();
	}
	else
	{
		HumanInput();
	}

	//move left
	if (m_keyState[LEFT_KEY])
	{
		if (!m_keyState[DOWN_KEY])
		{
			//change direction
			if (m_direction)
			{
				m_direction = false;
			}

			//change animation
			if (GetOnGround())
			{
				m_curSpr->ChangeAnim(LEFT);
			}

			// right key is pressed: move our character
			m_velocity.x = -m_moveSpeed;
		}
	}


	//move right
	if (m_keyState[RIGHT_KEY])
	{
		if (!m_keyState[DOWN_KEY])
		{
			//change direction
			if (!m_direction)
			{
				m_direction = true;
			}

			//change animation
			if (GetOnGround())
			{
				m_curSpr->ChangeAnim(RIGHT);
			}

			// right key is pressed: move our character
			m_velocity.x = m_moveSpeed;
		}
	}

	if ((m_keyState[LEFT_KEY] == false && m_keyState[RIGHT_KEY] == false))
	{
		m_velocity.x = 0.0f;
	}

	if (m_keyState[UP_KEY])
	{
		//change animation
		m_curSpr->ChangeAnim(LOOKUP);
	}

	//start change bbox to crouch bbox
		//if crouched key held down
	if (m_keyState[DOWN_KEY])
	{
		//if not changed bbox
		if (m_justCrouched == false)
		{
			//get current position
			sf::Vector2f pos = GetPosition();

			if (m_super)
			{
				m_curBBox = m_SCrouchBbox;

				//adjust bbox position
				if (m_direction)
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 1.f, m_curSpr->GetPosition().y + 22.f));
				else
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 1.f, m_curSpr->GetPosition().y + 22.f));
			}
			else
			{
				m_curBBox = m_CrouchBbox;

				//adjust bbox position
				if (m_direction)
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 2.f, m_curSpr->GetPosition().y + 12.f));
				else
					m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 2.f, m_curSpr->GetPosition().y + 12.f));
			}

			m_justCrouched = true;
			m_curSpr->ChangeAnim(CROUCH);
		}
	}
	else //if crouched key is not held down
	{
		//if was crouched
		if (m_justCrouched)
		{
			m_justCrouched = false;

			if (m_super)
				m_curBBox = m_SupBbox;
			else
				m_curBBox = m_bbox.get();

			SetPosition(GetPosition());
			m_curSpr->ChangeAnim(IDLE);
		}
	}//end change bbox to crouch bbox

	//regular jump
	if (m_keyState[SPACE_KEY])
	{
		if (m_cantjump == false)
		{
			if (GetOnGround())
			{
				//change animation
				if (!m_keyState[DOWN_KEY])
					m_curSpr->ChangeAnim(JUMP);
				// up key is pressed: move our character
				m_airbourne = true;
				m_onGround = false;
				m_velocity.y -= m_jumpSpeed;
				m_cantjump = true;
			}
		}
	}
	else
	{
		if (m_airbourne && m_cantjump)
		{
			m_curSpr->ChangeAnim(FALL);
			m_airtime = c_maxAirTime;
		}
	}

	//spin jump
	if (m_keyState[RCRTL_KEY])
	{
		if (m_cantSpinJump == false)
		{
			if (GetOnGround())
			{
				//change animation
				m_curSpr->ChangeAnim(SPINJUMP);
				// up key is pressed: move our character
				m_airbourne = true;
				m_onGround = false;
				m_velocity.y = -m_jumpSpeed;
				m_cantSpinJump = true;
			}
		}
	}
	else
	{
		if (m_airbourne && m_cantSpinJump)
		{
			m_curSpr->ChangeAnim(SPINJUMP);
			m_airtime = c_maxAirTime;
		}
	}

	if (!m_keyState[SPACE_KEY] && !m_keyState[RCRTL_KEY])
		m_cantjump = m_cantSpinJump = false;

	if (m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		if (!m_keyState[DOWN_KEY] && !m_keyState[UP_KEY])
			m_curSpr->ChangeAnim(IDLE);
	}
}
