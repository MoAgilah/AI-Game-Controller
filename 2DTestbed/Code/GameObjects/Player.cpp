#include "../GameObjects/Player.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../Controller/CtrlMgr.h"
#include <format>

bool Player::s_playerInserted = false;

Player::Player(int rows, int cols, bool symmetrical, int initAnim, float animSpd)
	: GameObject(TexID::Mario, rows, cols, (int)TexID::MarioBB, true, symmetrical, initAnim, animSpd)
{
	m_type = (int)TexID::Mario;
	m_spawnData.m_initialPos = sf::Vector2f(75, 454);

	m_keyStates.fill(false);

	std::vector<int> frames{ 1, 1, 1, 2, 1, 2, 1, 2 };
	//regular mario
	m_spr->SetFrames(frames);

	SetPosition(m_spawnData.m_initialPos);

	m_heightDiff = 11.25f;

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

void Player::Update(float deltaTime)
{
	if (!GetActive())
		return;

	//adjust position when changing from regular -> super and vice versa
	if (m_super)
	{
		//if current spr and bbox is not super
		if (m_spr->GetTexID() != TexID::Super)
		{
			//change spr and bbox
			m_spr->SetTexture(TexID::Super);
			auto frames = std::vector<int>{ 1, 1, 1, 3, 1, 2, 1, 2 };
			m_spr->SetFrameData(8, 3, frames);
			m_bbox->SetTexture(TexID::SuperBB);

			//adjust postion
			SetPosition(m_spr->GetPosition() - sf::Vector2f(0, m_heightDiff));
		}
	}
	else
	{
		//if current spr and bbox is not regular
		if (m_spr->GetTexID() != TexID::Mario)
		{
			//change spr and bbox
			m_spr->SetTexture(TexID::Mario);
			std::vector<int> frames{ 1, 1, 1, 2, 1, 2, 1, 2 };
			m_spr->SetFrameData(8, 2, frames);
			m_bbox->SetTexture(TexID::MarioBB);

			//adjust position
			SetPosition(m_spr->GetPosition() + sf::Vector2f(0, m_heightDiff));
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
			SetPrevPosition(m_spr->GetPosition());
			Move(sf::Vector2f(m_velocity.x * FPS * deltaTime, 0));
			Collisions::Get()->ProcessCollisions(this);
		}

		if (m_velocity.y != 0)
		{
			SetPrevPosition(m_spr->GetPosition());
			Move(sf::Vector2f(0, m_velocity.y * FPS * deltaTime));
			Collisions::Get()->ProcessCollisions(this);
		}
	}

	//check for leftmost and rightmost boundary
	if (m_spr->GetPosition().x < m_spr->GetOrigin().x || m_spr->GetPosition().x > 11776 - m_spr->GetOrigin().x)
	{
		Move(sf::Vector2f(-m_velocity.x * FPS * deltaTime, 0));
	}

	if (m_bbox->GetSprite()->getPosition().y > 600 - m_bbox->GetSprite()->getOrigin().y)
	{
		if (GetIsAlive())
			Kill();
	}

	//check for exceeded rightmost || or hit the goal
	if (m_spr->GetPosition().x > RightMost)
	{
		SetSpawnLoc();

		if (Automated == false)
		{
			Reset();
		}
	}

	m_spr->Update(deltaTime);
}

void Player::Render(sf::RenderWindow& window)
{
	m_spr->Render(window);
	m_bbox->Render(window);
}

void Player::Reset()
{
	if (m_spr->GetTexID() != TexID::Mario)
	{
		//change spr and bbox
		m_spr->SetTexture(TexID::Mario);
		std::vector<int> frames{ 1, 1, 1, 2, 1, 2, 1, 2 };
		m_spr->SetFrameData(8, 2, frames);
		m_bbox->SetTexture(TexID::MarioBB);

		//adjust position
		SetPosition(m_spr->GetPosition() + sf::Vector2f(0, m_heightDiff));
	}

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

	m_keyStates.fill(false);

	m_heightDiff = 0;
	m_noGravTime = 0;
	m_InvulTime = 0;
	m_airtime = 0;

	Timer::Get()->ResetTime();
	Game::GetGameMgr()->GetLevel()->ResetLevel();
}

void Player::Kill()
{
	m_airtime = 0.33f;
	m_airbourne = true;
	m_onGround = false;
	m_alive = false;
	m_spr->ChangeAnim(DIE);
}

void Player::Move(sf::Vector2f vel)
{
	m_spr->Move(vel.x, vel.y);
	m_bbox->GetSprite()->move(vel);
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

void Player::SetAirTime(float val)
{
	m_justHitEnemy = true;
	m_noGravTime = val;
}

void Player::JustBeenHit(bool hit)
{
	m_justBeenHit = hit;
	m_InvulTime = 1;
}

bool Player::UpdateANN()
{
	std::vector<double> inputs;

	inputs = CtrlMgr::GetCtrlMgr()->GetController()->GetGridInputs();

	outputs = m_itsBrain->Update(inputs, CNeuralNet::active);

	if (outputs.size() < CParams::iNumOutputs)
		return false;

	return true;
}

void Player::EndOfRunCalculations()
{
	float percent = ((GetPosition().x - 18.f) / RightMost) * 100;
	//completed level
	if (m_goalHit)
	{
		Game::GetGameMgr()->GetLogger()->AddExperimentLog("Completed the level");
		m_fitness += 1000;
	}
	else if (GetPosition().x <= 75.f)
	{
		m_fitness -= percent;

		if (GetPosition().x == 75.f)
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Did not move = {}% Completed", percent));
		}
		else
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Moved left  = {}% Completed", percent));
		}
	}
	//moved right some
	else
	{
		Game::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("{}% Completed", percent));
		m_fitness += percent;
	}
}

void Player::ControllerInput()
{
	Game::GetGameMgr()->GetLogger()->AddDebugLog(std::format("Player ", CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayerNum()), false);

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

		Game::GetGameMgr()->GetLogger()->AddDebugLog(std::format("{} = {} = {}", move, oval, output), false);
		Game::GetGameMgr()->GetLogger()->AddDebugLog("\t", false);
		//store output
		m_keyStates[i] = output;
	}
	Game::GetGameMgr()->GetLogger()->AddDebugLog("");
}

void Player::HumanInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_keyStates[LEFT_KEY] = true;
	}
	else
	{
		if (m_keyStates[LEFT_KEY])
		{
			m_keyStates[LEFT_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_keyStates[RIGHT_KEY] = true;
	}
	else
	{
		if (m_keyStates[RIGHT_KEY])
		{
			m_keyStates[RIGHT_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_keyStates[SPACE_KEY] = true;
	}
	else
	{
		if (m_keyStates[SPACE_KEY])
		{
			m_keyStates[SPACE_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		m_keyStates[RCRTL_KEY] = true;
	}
	else
	{
		if (m_keyStates[RCRTL_KEY])
		{
			m_keyStates[RCRTL_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_keyStates[UP_KEY] = true;
	}
	else
	{
		if (m_keyStates[UP_KEY])
		{
			m_keyStates[UP_KEY] = false;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_keyStates[DOWN_KEY] = true;
	}
	else
	{
		if (m_keyStates[DOWN_KEY])
		{
			m_keyStates[DOWN_KEY] = false;
		}
	}
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
	if (m_keyStates[LEFT_KEY])
	{
		if (!m_keyStates[DOWN_KEY])
		{
			//change direction
			if (GetDirection())
				SetDirection(false);

			//change animation
			if (GetOnGround())
			{
				m_spr->ChangeAnim(LEFT);
			}

			// right key is pressed: move our character
			m_velocity.x = -m_moveSpeed;
		}
	}


	//move right
	if (m_keyStates[RIGHT_KEY])
	{
		if (!m_keyStates[DOWN_KEY])
		{
			//change direction
			if (!GetDirection())
				SetDirection(true);

			//change animation
			if (GetOnGround())
			{
				m_spr->ChangeAnim(RIGHT);
			}

			// right key is pressed: move our character
			m_velocity.x = m_moveSpeed;
		}
	}

	if ((m_keyStates[LEFT_KEY] == false && m_keyStates[RIGHT_KEY] == false))
	{
		m_velocity.x = 0.0f;
	}

	if (m_keyStates[UP_KEY])
	{
		//change animation
		m_spr->ChangeAnim(LOOKUP);
	}

	//start change bbox to crouch bbox
		//if crouched key held down
	if (m_keyStates[DOWN_KEY])
	{
		//if not changed bbox
		if (m_justCrouched == false)
		{
			//get current position
			sf::Vector2f pos = GetPosition();

			if (m_super)
			{
				m_bbox->SetTexture(TexID::SuperSmlBB);

				//adjust bbox position
				if (GetDirection())
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 1.f, m_spr->GetPosition().y + 22.f));
				else
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 1.f, m_spr->GetPosition().y + 22.f));
			}
			else
			{
				m_bbox->SetTexture(TexID::MarioSmlBB);

				//adjust bbox position
				if (GetDirection())
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2.f, m_spr->GetPosition().y + 12.f));
				else
					m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2.f, m_spr->GetPosition().y + 12.f));
			}

			m_justCrouched = true;
			m_spr->ChangeAnim(CROUCH);
		}
	}
	else //if crouched key is not held down
	{
		//if was crouched
		if (m_justCrouched)
		{
			m_justCrouched = false;

			if (m_super)
				m_bbox->SetTexture(TexID::SuperBB);
			else
				m_bbox->SetTexture(TexID::MarioBB);

			SetPosition(GetPosition());
			m_spr->ChangeAnim(IDLE);
		}
	}//end change bbox to crouch bbox

	//regular jump
	if (m_keyStates[SPACE_KEY])
	{
		if (m_cantjump == false)
		{
			if (GetOnGround())
			{
				//change animation
				if (!m_keyStates[DOWN_KEY])
					m_spr->ChangeAnim(JUMP);
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
			m_spr->ChangeAnim(FALL);
			m_airtime = c_maxAirTime;
		}
	}

	//spin jump
	if (m_keyStates[RCRTL_KEY])
	{
		if (m_cantSpinJump == false)
		{
			if (GetOnGround())
			{
				//change animation
				m_spr->ChangeAnim(SPINJUMP);
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
			m_spr->ChangeAnim(SPINJUMP);
			m_airtime = c_maxAirTime;
		}
	}

	if (!m_keyStates[SPACE_KEY] && !m_keyStates[RCRTL_KEY])
		m_cantjump = m_cantSpinJump = false;

	if (m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		if (!m_keyStates[DOWN_KEY] && !m_keyStates[UP_KEY])
			m_spr->ChangeAnim(IDLE);
	}
}
