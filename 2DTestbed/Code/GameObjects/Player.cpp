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

Player::Player()
	: AnimatedObject(TexID::Mario, (int)TexID::MarioBB, true, false, Anims::IDLE, 0.5f)
{
	m_fragShader.loadFromFile("Resources/Shaders/FlashShader.frag", sf::Shader::Fragment);
	m_fragShader.setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));

	SetInitialPosition(sf::Vector2f(75, 454));
	SetPosition(GetInitialPosition());
	m_keyStates.fill(false);

	GetAnimSpr()->SetFrameData(14, 4, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });

	if (Automated)
	{
		if (s_playerInserted)
			Collisions::Get()->RemoveLastAdded();
	}

	s_playerInserted = true;
}

void Player::Update(float deltaTime)
{
	if (!GetActive())
		return;

	ProcessInput();

	m_stateMgr.Update(deltaTime);

	GetAnimSpr()->Update(deltaTime);

	if (GetIsAlive())
	{
		if (GetOnGround())
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

		//set vulnerability time
		if (m_justBeenHit)
		{
			m_InvulTime -= deltaTime;

			if (m_InvulTime <= 0)
			{
				m_InvulTime = 0;
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

		if ((m_keyStates[LEFT_KEY] == false && m_keyStates[RIGHT_KEY] == false))
			SetXVelocity(0.0f);

		if (!m_keyStates[JUMP_KEY] && !m_keyStates[SJUMP_KEY])
			m_cantjump = m_cantSpinJump = false;

		if (GetVelocity() == sf::Vector2f())
		{
			if (!m_keyStates[DOWN_KEY] && !m_keyStates[UP_KEY])
				GetAnimSpr()->ChangeAnim(IDLE);
		}

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(sf::Vector2f(GetXVelocity() * FPS * deltaTime, 0));
			Collisions::Get()->ProcessCollisions(this);
		}

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(sf::Vector2f(0, GetYVelocity() * FPS * deltaTime));
			Collisions::Get()->ProcessCollisions(this);
		}

		if (GetPosition().x < (GetOrigin().x * sX)*0.5)
		{
			Move(sf::Vector2f(-GetXVelocity() * FPS * deltaTime, 0));
		}

		if (GetPosition().x > RightMost)
		{
			SetSpawnLoc();

			if (Automated == false)
			{
				Reset();
			}
		}

		if (GetBBox()->GetSprite()->getPosition().y > 600 - GetBBox()->GetSprite()->getOrigin().y)
		{
			if (GetIsAlive())
				SetIsAlive(false);
		}
	}
	else
	{
		if (m_stateMgr.GetStateName() != "Dieing")
			m_stateMgr.ChangeState(new DieingState(this));
	}

	m_fragShader.setUniform("time", m_InvulTime);
}

void Player::Render(sf::RenderWindow& window)
{

	window.draw(*GetAnimSpr()->GetSprite(), &m_fragShader);
	GetBBox()->Render(window);
}

void Player::Reset()
{
	if (GetAnimSpr()->GetTexID() != TexID::Mario)
	{
		//change spr and bbox
		GetAnimSpr()->SetTexture(TexID::Mario);
		GetAnimSpr()->SetFrameData(14, 4, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });
		GetBBox()->SetTexture(TexID::MarioBB);

		//adjust position
		SetPosition(GetAnimSpr()->GetPosition() + sf::Vector2f(0, m_heightDiff));
	}

	GameObject::Reset();

	m_spawnLoc = GetInitialPosition();

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

void Player::Move(sf::Vector2f vel)
{
	GetAnimSpr()->Move(vel.x, vel.y);
	GetBBox()->GetSprite()->move(vel);
}

void Player::SetIsSuper(bool super)
{
	m_super = super;
	if (m_super)
	{
		//if current spr and bbox is not super
		if (GetAnimSpr()->GetTexID() != TexID::Super)
		{
			//change spr and bbox
			GetAnimSpr()->SetTexture(TexID::Super);
			GetAnimSpr()->SetFrameData(14, 4, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });
			GetBBox()->SetTexture(TexID::SuperBB);
			SetPosition(GetAnimSpr()->GetPosition() - sf::Vector2f(0, m_heightDiff));
		}
	}
	else
	{
		if (GetAnimSpr()->GetTexID() != TexID::Mario)
		{
			GetAnimSpr()->SetTexture(TexID::Mario);
			GetAnimSpr()->SetFrameData(14, 4, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });
			GetBBox()->SetTexture(TexID::MarioBB);
			SetPosition(GetAnimSpr()->GetPosition() + sf::Vector2f(0, m_heightDiff));
		}
	}
}

void Player::SetSpawnLoc(sf::Vector2f loc)
{
	if (loc == sf::Vector2f(0, 0))
	{
		m_spawnLoc = GetInitialPosition();
	}
	else
	{
		m_spawnLoc.x = loc.x;
	}
}

void Player::ForceFall()
{
	SetCantJump(true);
	SetCantSpinJump(true);
	SetAirbourne(false);
}

void Player::JusyHitEnemy(float val)
{
	m_justHitEnemy = true;
	m_noGravTime = val;
}

void Player::JustBeenHit(bool hit)
{
	m_justBeenHit = hit;
	m_InvulTime = 1.f;
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

void Player::ControllerInput()
{
	Game::GetGameMgr()->GetLogger()->AddDebugLog(std::format("Player {}", CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayerNum()), false);

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
		case SJUMP_KEY:
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
		m_keyStates[SJUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[SJUMP_KEY])
			m_keyStates[SJUMP_KEY] = false;
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
				GetBBox()->SetTexture(TexID::MarioSmlBB);

				//adjust bbox position
				if (GetDirection())
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x - 1.f, GetAnimSpr()->GetPosition().y + 22.f));
				else
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x + 1.f, GetAnimSpr()->GetPosition().y + 22.f));
			}
			else
			{
				GetBBox()->SetTexture(TexID::MarioSmlBB);

				//adjust bbox position
				if (GetDirection())
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x - 2.f, GetAnimSpr()->GetPosition().y + 12.f));
				else
					GetBBox()->Update(sf::Vector2f(GetAnimSpr()->GetPosition().x + 2.f, GetAnimSpr()->GetPosition().y + 12.f));
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
				GetBBox()->SetTexture(TexID::SuperBB);
			else
				GetBBox()->SetTexture(TexID::MarioBB);

			SetPosition(GetPosition());
		}
	}
}
