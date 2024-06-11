#include "../GameObjects/Player.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../Controller/CtrlMgr.h"
#include <format>
#include <iostream>
#include "../GameObjectState/PlayerState.h"

bool Player::s_playerInserted = false;

Player::Player(int rows, int cols, bool symmetrical, int initAnim, float animSpd)
	: AnimatedGameObject(TexID::Mario, rows, cols, (int)TexID::MarioBB, true, symmetrical, initAnim, animSpd)
{
	m_type = (int)TexID::Mario;
	m_spawnData.m_initialPos = sf::Vector2f(75, 454);
	m_keyStates.fill(false);
	SetPosition(m_spawnData.m_initialPos);
	GetAnimSpr()->SetFrameData(rows, cols, { 1, 1, 1, 2, 1, 2, 1, 2 });

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
			GetAnimSpr()->SetFrameData(8, 3, { 1, 1, 1, 3, 1, 2, 1, 2 });
			m_bbox->SetTexture(TexID::SuperBB);
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
			GetAnimSpr()->SetFrameData(8, 2, { 1, 1, 1, 2, 1, 2, 1, 2 });
			m_bbox->SetTexture(TexID::MarioBB);
			SetPosition(m_spr->GetPosition() + sf::Vector2f(0, m_heightDiff));
		}

	}//end super

	ProcessInput();

	if (GetIsAlive())
	{
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
				SetYVelocity(0);
				m_noGravTime -= deltaTime;
			}
		}

		if (GetOnGround())
		{
			if (!GetAirbourne())
			{
				if (m_stateMgr.GetStateName() == "Airborne")
					m_stateMgr.PopState();

				if (GetIsCrouched())
				{
					if (m_stateMgr.GetStateName() != "Crouching")
						m_stateMgr.ChangeState(new CrouchingState(this));
				}
				else
				{
					if (m_stateMgr.GetStateName() != "Grounded")
						m_stateMgr.ChangeState(new GroundedState(this));
				}

				SetYVelocity(0);
				m_airtime = 0;
			}
		}
		else
		{
			if (GetAirbourne())
			{
				if (m_stateMgr.GetStateName() != "Airborne")
					m_stateMgr.PushState(new AirborneState(this));

				m_airtime += deltaTime;
				if (m_airtime >= c_maxAirTime)
					SetAirbourne(false);
			}
			else
			{
				IncrementYVelocity(c_gravity);
			}
		}

		if ((m_keyStates[LEFT_KEY] == false && m_keyStates[RIGHT_KEY] == false))
			m_velocity.x = 0.0f;

		if (!m_keyStates[JUMP_KEY] && !m_keyStates[SPINJUMP])
			m_cantjump = m_cantSpinJump = false;

		if (GetVelocity() == sf::Vector2f())
		{
			if (!m_keyStates[DOWN_KEY] && !m_keyStates[UP_KEY])
				GetAnimSpr()->ChangeAnim(IDLE);
		}

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(m_spr->GetPosition());
			Move(sf::Vector2f(GetXVelocity() * FPS * deltaTime, 0));
			Collisions::Get()->ProcessCollisions(this);
		}

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(m_spr->GetPosition());
			Move(sf::Vector2f(0, GetYVelocity() * FPS * deltaTime));
			Collisions::Get()->ProcessCollisions(this);
		}
	}
	else
	{
		if (m_stateMgr.GetStateName() != "Dieing")
			m_stateMgr.ChangeState(new DieingState(this));
	}

	if (m_spr->GetPosition().x < LeftMost)
		Move(sf::Vector2f(-GetXVelocity() * FPS * deltaTime, 0));

	if (m_spr->GetPosition().x > RightMost)
	{
		SetSpawnLoc();

		if (Automated == false)
		{
			Reset();
		}
	}

	if (m_bbox->GetSprite()->getPosition().y > 600 - m_bbox->GetSprite()->getOrigin().y)
	{
		if (GetIsAlive())
			SetIsAlive(false);
	}

	m_stateMgr.Update(deltaTime);

	GetAnimSpr()->Update(deltaTime);
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
		GetAnimSpr()->SetFrameData(8, 2, frames);
		m_bbox->SetTexture(TexID::MarioBB);

		//adjust position
		SetPosition(m_spr->GetPosition() + sf::Vector2f(0, m_heightDiff));
	}

	GameObject::Reset();

	m_spawnLoc = m_spawnData.m_initialPos;

	SetVelocity(sf::Vector2f(0.0f, 0.0f));

	m_super = false;
	m_crouched = false;
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

	m_stateMgr.ClearStates();
	Timer::Get()->ResetTime();
	Game::GetGameMgr()->GetLevel()->ResetLevel();
}

void Player::Kill()
{
	m_airtime = 0.33f;
	SetAirbourne(true);
	SetOnGround(false);
	m_alive = false;
	GetAnimSpr()->ChangeAnim(DIE);
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

void Player::ForceFall()
{
	if (m_cantjump == false || m_cantSpinJump == false)
	{
		m_cantjump = true;
		m_cantSpinJump = true;

		if (GetAirbourne())
		{
			SetAirbourne(false);
		}
	}
}

void Player::JusyHitEnemy(float val)
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
	if (GetGoalHit())
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
		case JUMP_KEY:
			move = "jump";
			break;
		case SPINJUMP:
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
	{
		m_keyStates[LEFT_KEY] = true;
	}
	else
	{
		if (m_keyStates[LEFT_KEY])
			m_keyStates[LEFT_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
	{
		m_keyStates[RIGHT_KEY] = true;
	}
	else
	{
		if (m_keyStates[RIGHT_KEY])
			m_keyStates[RIGHT_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_keyStates[JUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[JUMP_KEY])
			m_keyStates[JUMP_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_keyStates[SPINJUMP] = true;
	}
	else
	{
		if (m_keyStates[SPINJUMP])
			m_keyStates[SPINJUMP] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
	{
		m_keyStates[UP_KEY] = true;
	}
	else
	{
		if (m_keyStates[UP_KEY])
			m_keyStates[UP_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
	{
		m_keyStates[DOWN_KEY] = true;
	}
	else
	{
		if (m_keyStates[DOWN_KEY])
			m_keyStates[DOWN_KEY] = false;
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

	m_stateMgr.ProcessInputs();

	if (m_keyStates[DOWN_KEY])
	{
		if (!GetIsCrouched())
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

			SetIsCrouched(true);
		}
	}
	else
	{
		//if was crouched
		if (GetIsCrouched())
		{
			SetIsCrouched(false);

			if (m_super)
				m_bbox->SetTexture(TexID::SuperBB);
			else
				m_bbox->SetTexture(TexID::MarioBB);

			SetPosition(GetPosition());
		}
	}
}
