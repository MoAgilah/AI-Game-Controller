#include "Player.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../Controller/CtrlMgr.h"
#include <format>
#include <iostream>
#include "../GameStates/PlayerState.h"

Player::Player(const sf::Vector2f& pos)
	: DynamicObject(new AnimatedSprite(TexID::Mario, 14, 4, FPS, false, 0.5f), TexID::MarioBB)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));

	m_keyStates.fill(false);

	m_fragShader.loadFromFile("Resources/Shaders/FlashShader.frag", sf::Shader::Fragment);
	m_fragShader.setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));

	std::vector<int> frames{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void Player::Update(float deltaTime)
{
	if (!GetActive())
		return;

	ProcessInput();

	UpdateBoundingBox();

	m_stateMgr.Update(deltaTime);

	static_cast<AnimatedSprite*>(GetSprite())->Update(deltaTime);

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
				static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(IDLE);
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

		if (GetPosition().x < (GetOrigin().x * sX) * 0.5)
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
	window.draw(*GetSprite()->GetSprite(), &m_fragShader);
	GetBBox()->Render(window);
}

void Player::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(0);
	if (GetSprite()->GetTexID() != TexID::Mario)
	{
		//change spr and bbox
		GetSprite()->SetTexture(TexID::Mario);
		GetSprite()->SetFrameSize(sf::Vector2u(GetSprite()->GetTextureSize().x / 4, GetSprite()->GetTextureSize().y / 14));
		GetBBox()->SetTexture(TexID::MarioBB);

		//adjust position
		SetPosition(GetPosition() + sf::Vector2f(0, m_heightDiff));
	}

	DynamicObject::Reset();

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

void Player::SetIsSuper(bool super)
{
	m_super = super;
	if (m_super)
	{
		//if current spr and bbox is not super
		if (GetSprite()->GetTexID() != TexID::Super)
		{
			//change spr and bbox
			GetSprite()->SetTexture(TexID::Super);
			GetSprite()->SetFrameSize(sf::Vector2u(GetSprite()->GetTextureSize().x / 4, GetSprite()->GetTextureSize().y / 14));
			GetBBox()->SetTexture(TexID::SuperBB);
			SetPosition(GetPosition() - sf::Vector2f(0, m_heightDiff));
		}
	}
	else
	{
		if (GetSprite()->GetTexID() != TexID::Mario)
		{
			GetSprite()->SetTexture(TexID::Mario);
			GetSprite()->SetFrameSize(sf::Vector2u(GetSprite()->GetTextureSize().x / 4, GetSprite()->GetTextureSize().y / 14));
			GetBBox()->SetTexture(TexID::MarioBB);
			SetPosition(GetPosition() + sf::Vector2f(0, m_heightDiff));
		}
	}
}

void Player::SetKeyState(int index, bool val)
{
	if (index < m_keyStates.size())
		m_keyStates[index] = val;
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

void Player::JustBeenHit(bool hit)
{
	m_justBeenHit = hit;
	m_InvulTime = 1.f;
}

void Player::JusyHitEnemy(float val)
{
	m_justHitEnemy = true;
	m_noGravTime = val;
}

void Player::ResolveCollisions(Object* other)
{
}

void Player::ResolveCollisions(Tile* tile)
{

}

void Player::UpdateBoundingBox()
{
	if (GetIsCrouched())
	{
		if (GetIsSuper())
		{
			if (GetDirection())
				GetBBox()->Update(sf::Vector2f(GetPosition().x - 1.f, GetPosition().y + 22.f));
			else
				GetBBox()->Update(sf::Vector2f(GetPosition().x + 1.f, GetPosition().y + 22.f));
		}
		else
		{
			//adjust bbox position
			if (GetDirection())
				GetBBox()->Update(sf::Vector2f(GetPosition().x - 2.f, GetPosition().y + 12.f));
			else
				GetBBox()->Update(sf::Vector2f(GetPosition().x + 2.f, GetPosition().y + 12.f));
		}
	}
	else
	{
		GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	}
}

void Player::ProcessInput()
{
	if (!GetIsAlive())
		return;

	Input();

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
			}
			else
			{
				GetBBox()->SetTexture(TexID::MarioSmlBB);
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

void Player::Input()
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

bool AutomatedPlayer::s_playerInserted = false;

AutomatedPlayer::AutomatedPlayer(const sf::Vector2f& pos)
	: Player(pos)
{
	if (s_playerInserted)
		Collisions::Get()->RemoveLastAdded();

	s_playerInserted = true;
}

bool AutomatedPlayer::UpdateANN()
{
	std::vector<double> inputs;

	inputs = CtrlMgr::GetCtrlMgr()->GetController()->GetGridInputs();

	outputs = m_itsBrain->Update(inputs, CNeuralNet::active);

	if (outputs.size() < CParams::iNumOutputs)
		return false;

	return true;
}

void AutomatedPlayer::Input()
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
		SetKeyState(i, output);
	}

	Game::GetGameMgr()->GetLogger()->AddDebugLog("");
}
