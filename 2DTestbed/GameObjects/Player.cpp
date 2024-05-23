#include "../GameObjects/Player.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../Controller/CtrlMgr.h"

bool Player::s_playerInserted = false;

Player::Player(std::string filepath, int rows, int cols, bool symmetrical, int initAnim, float animSpd)
	: GameObject(filepath, rows, cols, PLAYER, true, symmetrical, initAnim, animSpd)
{
	m_type = PLAYER;

	std::vector<int> frames{ 1, 1, 1, 2, 1, 2, 1, 2 };
	//regular mario
	m_spr->SetFrames(frames);
	m_spr->SetPosition(sf::Vector2f(75, 454));
	m_bbox->GetSprite()->setPosition(sf::Vector2f(m_spr->GetPosition().x - 2, m_spr->GetPosition().y + 4));
	m_CrouchBbox = new BoundingBox("smlCrouch", m_type);

	m_currSpr = m_spr;
	m_curBbox = m_bbox;

	m_deathLoc = m_velocity = sf::Vector2f(0.0f, 0.0f);
	m_prevPos = m_spawnLoc = m_initialPos = sf::Vector2f{ 75, 454 };

	//super mario
	filepath = "s" + filepath;
	frames = std::vector<int>{ 1, 1, 1, 3, 1, 2, 1, 2 };
	m_SupSpr = new AnimatedSprite(filepath, rows, cols + 1, FPS, symmetrical, initAnim, animSpd);
	m_SupSpr->SetFrames(frames);
	m_SupSpr->SetPosition(sf::Vector2f(m_SupSpr->GetOrigin().x * 2, 481));

	m_SupBbox = new BoundingBox(filepath.substr(0, filepath.find(".")), PLAYER);
	m_SCrouchBbox = new BoundingBox("supCrouch", PLAYER);

	m_onGround = false;
	m_falling = false;
	m_airbourne = false;
	m_spindown = false;
	ifWasSuper = m_super = false;
	justCrouched = false;
	m_visible = true;
	die = false;
	killed = false;
	m_alive = true;
	cantjump = false;
	cantSpinJump = false;
	justBeenHit = false;
	justHitEnemy = false;
	goalHit = false;

	m_airtime = 0;
	m_noGravTime = -1;
	m_InvulTime = -1;

	heightDiff = m_SupSpr->GetOrigin().y * sY - m_spr->GetOrigin().y * sY;



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

	ProcessInput();

	if (!GetOnGround())
	{
		if (m_airbourne)
		{
			m_airtime += deltaTime;
			if (m_airtime >= c_maxAirTime)
			{
				m_airbourne = false;
				justCrouched = false;
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
		SetPrevPosition(m_currSpr->GetPosition());
		Move(sf::Vector2f(m_velocity.x * FPS * deltaTime, 0));
		Collisions::Get()->ProcessCollisions(this);
	}

	if (m_velocity.y != 0)
	{
		SetPrevPosition(m_currSpr->GetPosition());
		Move(sf::Vector2f(0, m_velocity.y * FPS * deltaTime));
		Collisions::Get()->ProcessCollisions(this);
	}

	//check for leftmost and rightmost boundary
	if (m_currSpr->GetPosition().x < m_currSpr->GetOrigin().x || m_currSpr->GetPosition().x > 11776 - m_currSpr->GetOrigin().x)
	{
		Move(sf::Vector2f(-m_velocity.x * FPS * deltaTime, 0));
	}

	//check for exceeded rightmost || or hit the goal
	if (m_currSpr->GetPosition().x > RightMost || goalHit)
	{
		goalHit = false;

		CheckPointHit(false);
		SetSpawnLoc();

		if (Automated == false)
		{
			ReSpawn();
		}
	}

	m_currSpr->Update(deltaTime, m_direction);
}

void Player::Render(sf::RenderWindow& window)
{
	m_currSpr->Render(window);
}

void Player::Move(sf::Vector2f vel)
{
	m_currSpr->Move(vel.x, vel.y);
	m_curBbox->GetSprite()->move(vel);
}

BoundingBox* Player::GetBBox()
{
	return m_curBbox;
}

sf::Vector2f Player::GetPosition() const
{
	return m_currSpr->GetPosition();
}

void Player::SetPosition(sf::Vector2f pos)
{
	m_currSpr->SetPosition(pos);
	if (m_direction)
	{
		//+
		m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x - 2.f, m_currSpr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x + 2.f, m_currSpr->GetPosition().y + 3.5f));
	}
}

void Player::SetPosition(float x, float y)
{
	m_currSpr->SetPosition(sf::Vector2f(x, y));
	if (m_direction)
	{
		//+
		m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x - 2.f, m_currSpr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x + 2.f, m_currSpr->GetPosition().y + 3.5f));
	}
}

sf::Vector2f Player::GetOrigin() const
{
	return m_currSpr->GetOrigin();
}

void Player::SetPrevPosition(sf::Vector2f pos)
{
	m_prevPos = pos;
}

void Player::SetPrevPosition(float x, float y)
{
	m_prevPos = sf::Vector2f(x, y);
}

sf::Vector2f Player::GetPrevPostion()
{
	return m_prevPos;
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
		m_spawnLoc = m_initialPos;
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
	ifWasSuper = m_super = false;

	m_currSpr = m_spr;
	m_curBbox = m_bbox;

	m_spawnLoc = m_initialPos;

	SetPosition(m_spawnLoc);
	SetPrevPosition(m_spawnLoc);

	m_velocity = sf::Vector2f(0.0f, 0.0f);

	m_airtime = 0;

	m_onGround = true;
	m_falling = false;
	m_airbourne = false;
	m_spindown = false;
	justCrouched = false;
	die = false;
	killed = false;
	m_alive = true;

	m_active = m_visible = true;
	m_currSpr->ChangeAnim(m_initialAnim);

	for (size_t i = 0; i < MAXKEYS; i++)
	{
		m_keyState[i] = false;
	}
}

void Player::ReSpawn()
{
	ifWasSuper = m_super = false;

	m_currSpr = m_spr;
	m_curBbox = m_bbox;

	SetPosition(m_spawnLoc);
	SetPrevPosition(m_spawnLoc);

	m_velocity = sf::Vector2f(0.0f, 0.0f);

	m_airtime = 0;

	m_onGround = true;
	m_falling = false;
	m_airbourne = false;
	m_spindown = false;
	justCrouched = false;
	die = false;
	killed = false;
	m_alive = true;

	m_active = m_visible = true;
	m_currSpr->ChangeAnim(m_initialAnim);
	Timer::Get()->ResetTime();
	Game::GetGameMgr()->GetLevel()->ResetLevel();
}

void Player::CheckPointHit(bool hit)
{
	hitChkPnt = hit;
}

void Player::SetAirTime(float val)
{
	justHitEnemy = true;
	m_noGravTime = val;
}

void Player::JustBeenHit(bool hit)
{
	justBeenHit = hit;
	m_InvulTime = 1;
}

bool Player::GetIfInvulnerable()
{
	return justBeenHit;
}

void Player::Kill()
{
	m_deathLoc = GetPosition() - sf::Vector2f(0, 20);
	killed = true;
	m_goingUp = true;
	die = true;
	m_alive = false;
	m_currSpr->ChangeAnim(DIE);//Die
	//m_dFitness -= 200;
}

bool Player::GetGoalHit()
{
	return goalHit;
}

void Player::GoalHit()
{
	goalHit = true;;
}

bool Player::GetIsAlive()
{
	return m_alive;
}

void Player::SetCantJump()
{
	if (cantjump == false || cantSpinJump == false)
	{
		cantjump = true;
		cantSpinJump = true;

		if (m_airbourne)
		{
			m_airbourne = false;
		}

		if (m_falling == false)
		{
			m_falling = true;
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
	if (goalHit)
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
				m_currSpr->ChangeAnim(LEFT);
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
				m_currSpr->ChangeAnim(RIGHT);
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
		m_currSpr->ChangeAnim(LOOKUP);
	}

	//start change bbox to crouch bbox
		//if crouched key held down
	if (m_keyState[DOWN_KEY])
	{
		//if not changed bbox
		if (justCrouched == false)
		{
			//get current position
			sf::Vector2f pos = GetPosition();

			if (m_super)
			{
				m_curBbox = m_SCrouchBbox;

				//adjust bbox position
				if (m_direction)
					m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x - 1.f, m_currSpr->GetPosition().y + 22.f));
				else
					m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x + 1.f, m_currSpr->GetPosition().y + 22.f));
			}
			else
			{
				m_curBbox = m_CrouchBbox;

				//adjust bbox position
				if (m_direction)
					m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x - 2.f, m_currSpr->GetPosition().y + 12.f));
				else
					m_curBbox->Update(sf::Vector2f(m_currSpr->GetPosition().x + 2.f, m_currSpr->GetPosition().y + 12.f));
			}

			justCrouched = true;
			m_currSpr->ChangeAnim(CROUCH);
		}
	}
	else //if crouched key is not held down
	{
		//if was crouched
		if (justCrouched)
		{
			justCrouched = false;

			if (m_super)
				m_curBbox = m_SupBbox;
			else
				m_curBbox = m_bbox;

			SetPosition(GetPosition());
			m_currSpr->ChangeAnim(IDLE);
		}
	}//end change bbox to crouch bbox

	//regular jump
	if (m_keyState[SPACE_KEY])
	{
		if (cantjump == false)
		{
			if (GetOnGround())
			{
				//change animation
				if (!m_keyState[DOWN_KEY])
					m_currSpr->ChangeAnim(JUMP);
				// up key is pressed: move our character
				m_airbourne = true;
				m_onGround = false;
				m_velocity.y -= m_jumpSpeed;
				cantjump = true;
			}
		}
	}
	else
	{
		if (m_airbourne && cantjump)
		{
			m_currSpr->ChangeAnim(FALL);
			m_airtime = c_maxAirTime;
		}
	}

	//spin jump
	if (m_keyState[RCRTL_KEY])
	{
		if (cantSpinJump == false)
		{
			if (GetOnGround())
			{
				//change animation
				m_currSpr->ChangeAnim(SPINJUMP);
				// up key is pressed: move our character
				m_airbourne = true;
				m_onGround = false;
				m_velocity.y = -m_jumpSpeed;
				cantSpinJump = true;
			}
		}
	}
	else
	{
		if (m_airbourne && cantSpinJump)
		{
			m_currSpr->ChangeAnim(SPINJUMP);
			m_airtime = c_maxAirTime;
		}
	}

	if (!m_keyState[SPACE_KEY] && !m_keyState[RCRTL_KEY])
		cantjump = cantSpinJump = false;

	if (m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		if (!m_keyState[DOWN_KEY] && !m_keyState[UP_KEY])
			m_currSpr->ChangeAnim(IDLE);
	}
}
