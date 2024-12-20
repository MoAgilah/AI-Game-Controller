#include "Player.h"
#include <format>
#include <iostream>
#include "../NEAT/phenotype.h"
#include "../Game/GameManager.h"
#include "../GameStates/PlayerState.h"

Player::Player(const sf::Vector2f& pos)
	: DynamicObject(new AnimatedSprite(TexID::Mario, 14, 4, GameConstants::FPS, false, 0.5f), sf::Vector2f(9,16)), m_airTimer(0.4f), m_invulTimer(1)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
	SetSpawnLoc();
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	GetAnimSpr()->SetFrames({ 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4 });

	m_fragShader = GameManager::Get()->GetShaderMgr().GetShader(ShaderID::Flash);
	m_fragShader->setUniform("flashColor", sf::Glsl::Vec4(1, 1, 1, 1));
	m_invulTimer.SetTime(0);
}

void Player::Update(float deltaTime)
{
	AnimatedSprite* animSpr = GetAnimSpr();
	PhysicsController* physCtrl = GetPhysicsController();
	GameManager* gameMgr = GameManager::Get();

	ProcessInput();

	m_stateMgr.Update(deltaTime);
	animSpr->Update(deltaTime);
	physCtrl->Update(GetDirection(), GetVelocity());

	if (GetIsAlive())
	{
		auto& inputManager = gameMgr->GetInputManager();

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

			if (GetOnSlope())
			{
				if (physCtrl->GetPhysicsType() != PhysicsType::slope)
					physCtrl->SetOnSlope();

				if (!GetXVelocity())
				{
					if (GetShouldSlideLeft())
						SetSlideLeft(true);

					if (GetShouldSlideRight())
						SetSlideRight(true);
				}

				if (GetSlideLeft() || GetSlideRight())
				{
					if (inputManager.GetKeyState(Keys::LEFT_KEY) || inputManager.GetKeyState(Keys::RIGHT_KEY))
					{
						SetSlideLeft(false);
						SetSlideRight(false);
					}
					else
					{
						if (GetSlideLeft())
							DecrementXVelocity(physCtrl->GetXAcceleration());

						if (GetSlideRight())
							IncrementXVelocity(physCtrl->GetXAcceleration());

						if (animSpr->GetCurrentAnim() != MarioAnims::IDLE)
							animSpr->ChangeAnim(MarioAnims::IDLE);
					}
				}
			}
			else
			{
				if (physCtrl->GetPhysicsType() != PhysicsType::ground)
					physCtrl->SetWalking();

				SetSlideLeft(false);
				SetSlideRight(false);
				SetShouldSlideLeft(false);
				SetShouldSlideRight(false);
			}

			SetYVelocity(0);

			if (!GetXVelocity() && !GetYVelocity())
			{
				if (!inputManager.GetKeyState(Keys::DOWN_KEY) && !inputManager.GetKeyState(Keys::UP_KEY))
				{
					if (animSpr->GetCurrentAnim() != MarioAnims::IDLE)
						animSpr->ChangeAnim(MarioAnims::IDLE);
				}
			}
		}
		else
		{
			if (GetAirbourne())
			{
				if (m_stateMgr.GetStateName() != "Airborne")
					m_stateMgr.PushState(new AirborneState(this));

				m_airTimer.Update(deltaTime);
				if (m_airTimer.CheckEnd())
				{
					SetAirbourne(false);
					m_airTimer.ResetTime();
				}
				else
				{
					if (m_airTimer.GetTime() < physCtrl->GetAirTime() * 0.6f)
						DecrementYVelocity(physCtrl->GetYAcceleration());
				}
			}
			else
			{
				IncrementYVelocity(GameConstants::Gravity);
				if (!GetOnSlope())
				{
					if (physCtrl->GetPhysicsType() != PhysicsType::drop)
					{
						if (!GetIsCrouched())
						{
							if (animSpr->GetCurrentAnim() != MarioAnims::SPINJUMP)
								animSpr->ChangeAnim(FALL);
						}

						physCtrl->SetFalling();
					}
				}
			}
		}

		if ((!inputManager.GetKeyState(Keys::LEFT_KEY) && !inputManager.GetKeyState(Keys::RIGHT_KEY)) && (!GetSlideLeft() && !GetSlideRight()))
			SetXVelocity(0.0f);

		//decomposition of movement
		if (GetXVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(sf::Vector2f(GetXVelocity() * GameConstants::FPS * deltaTime, 0));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
		}

		CheckForHorizontalBounds(deltaTime);
		SetGoalHit(GetPosition().x > GameConstants::RightMost);

		if (GetGoalHit())
		{
			SetSpawnLoc();

			if (!GameConstants::Automated)
				Reset();
		}

		if (GetYVelocity() != 0)
		{
			SetPrevPosition(GetPosition());
			Move(sf::Vector2f(0, GetYVelocity() * GameConstants::FPS * deltaTime));
			gameMgr->GetCollisionMgr()->ProcessCollisions(this);
		}


		if (gameMgr->GetCamera().CheckVerticalBounds(GetAABB()))
			SetIsAlive(false, 0.4f);

		if (gameMgr->GetTimer().CheckEnd())
			SetIsAlive(false);

		if (GetIfInvulnerable())
		{
			m_invulTimer.Update(deltaTime);
			m_fragShader->setUniform("time", m_invulTimer.GetTime());
		}
	}
	else
	{
		if (m_stateMgr.GetStateName() != "Dieing")
			m_stateMgr.ChangeState(new DieingState(this));
	}
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(*GetSprite()->GetSprite(), m_fragShader);
#if defined _DEBUG
	GetAABB()->Render(window);
#endif
}

void Player::Reset()
{
	SetInitialPosition(m_spawnLoc);
	DynamicObject::Reset();
	SetIsSuper(false);
	auto animSpr = GetAnimSpr();
	if (animSpr->GetCurrentAnim() != MarioAnims::IDLE)
		GetAnimSpr()->ChangeAnim(MarioAnims::IDLE);

	m_crouched = false;
	m_alive = true;
	m_cantjump = false;
	m_cantSpinJump = false;
	m_goalHit = false;

	m_invulTimer.SetTime(0);
	m_airTimer.ResetTime();
	m_stateMgr.ClearStates();
	GameManager::Get()->GetTimer().ResetTime();
	GameManager::Get()->GetWorld()->ResetLevel();
}

void Player::SetIsSuper(bool super)
{
	m_super = super;
	if (m_super)
	{
		if (GetSprite()->GetTexID() != TexID::Super)
		{
			GetSprite()->SetTexture(TexID::Super);
			GetSprite()->SetFrameSize(sf::Vector2u(GetSprite()->GetTextureSize().x / 4, GetSprite()->GetTextureSize().y / 14));
			GetAABB()->Reset(m_boxSizes[MarioBoxes::SUPER]);
			Move(0, -m_heightDiff);
		}
	}
	else
	{
		if (GetSprite()->GetTexID() != TexID::Mario)
		{
			GetSprite()->SetTexture(TexID::Mario);
			GetSprite()->SetFrameSize(sf::Vector2u(GetSprite()->GetTextureSize().x / 4, GetSprite()->GetTextureSize().y / 14));
			GetAABB()->Reset(m_boxSizes[MarioBoxes::REGULAR]);
			Move(0, m_heightDiff);
		}
	}
}

void Player::SetIsAlive(bool val, float airtime)
{
	m_alive = val;
	if (!m_alive)
	{
		m_airTimer.SetTime(airtime);
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

void Player::SetIsCrouched(bool crouched)
{
	m_crouched = crouched;
	if (m_crouched)
	{
		GetAABB()->Reset(m_boxSizes[MarioBoxes::CROUCHED]);
		if (m_super)
		{
			GetAABB()->Update(GetAABB()->GetPosition() + sf::Vector2f(0, 15));
		}
		else
		{
			GetAABB()->Update(GetAABB()->GetPosition() + sf::Vector2f(0, 5));
		}
	}
	else
	{
		if (m_super)
		{
			GetAABB()->Reset(m_boxSizes[MarioBoxes::SUPER]);
			GetAABB()->Update(GetAABB()->GetPosition() - sf::Vector2f(0, 15));
		}
		else
		{
			GetAABB()->Reset(m_boxSizes[MarioBoxes::REGULAR]);
			GetAABB()->Update(GetAABB()->GetPosition() - sf::Vector2f(0, 5));
		}
	}
}

void Player::ForceFall()
{
	SetCantJump(true);
	SetCantSpinJump(true);
	SetAirbourne(false);
}

void Player::Bounce()
{
	SetYVelocity(-(GetYVelocity() / 2));
	SetXVelocity(GetDirection() ? GetXVelocity() : -GetXVelocity());
}

void Player::SetInvulnerability()
{
	m_invulTimer.ResetTime();
}

void Player::SetAirbourne(bool air)
{
	m_airbourne = air;
	if (m_airbourne)
		SetOnGround(false);
}

void Player::ProcessInput()
{
	if (!GetIsAlive())
		return;

	AnimatedSprite* animSpr = GetAnimSpr();

	Input();

	m_stateMgr.ProcessInputs();

	if (GameManager::Get()->GetInputManager().GetKeyState(Keys::DOWN_KEY))
	{
		if (GetOnSlope())
		{
			if (animSpr->GetCurrentAnim() != MarioAnims::SLIDE)
				animSpr->ChangeAnim(MarioAnims::SLIDE);
		}
		else
		{
			if (!GetIsCrouched())
				SetIsCrouched(true);
		}
	}
	else
	{
		if (GetOnSlope())
		{
			if (animSpr->GetCurrentAnim() == MarioAnims::SLIDE)
				animSpr->ChangeAnim(MarioAnims::IDLE);
		}
		else
		{
			if (GetIsCrouched())
				SetIsCrouched(false);
		}
	}
}

void Player::Input()
{
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		inputManager.GetKeyState(Keys::LEFT_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::LEFT_KEY))
			inputManager.GetKeyState(Keys::LEFT_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		inputManager.GetKeyState(Keys:RIGHT_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys:RIGHT_KEY))
			inputManager.GetKeyState(Keys:RIGHT_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		inputManager.GetKeyState(Keys::UP_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::UP_KEY))
			inputManager.GetKeyState(Keys::UP_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		inputManager.GetKeyState(Keys::DOWN_KEY) = true;
	}
	else
	{
		if (inputManager.GetKeyState(Keys::DOWN_KEY))
			inputManager.GetKeyState(Keys::DOWN_KEY) = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_keyStates[Keys::JUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::JUMP_KEY])
			m_keyStates[Keys::JUMP_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_keyStates[Keys::RUN_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::RUN_KEY])
			m_keyStates[Keys::RUN_KEY] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_keyStates[Keys::SJUMP_KEY] = true;
	}
	else
	{
		if (m_keyStates[Keys::SJUMP_KEY])
			m_keyStates[Keys::SJUMP_KEY] = false;
	}*/
}

bool AutomatedPlayer::s_playerInserted = false;

AutomatedPlayer::AutomatedPlayer(const sf::Vector2f& pos)
	: Player(pos)
{
	if (s_playerInserted)
		GameManager::Get()->GetCollisionMgr()->RemoveLastAdded();

	s_playerInserted = true;
}

void AutomatedPlayer::Reset()
{
	Player::Reset();
	m_fitness = 0;
}

bool AutomatedPlayer::UpdateANN()
{
	std::vector<double> inputs;

	inputs = GameManager::Get()->GetAIController()->GetGridInputs();

	outputs = m_itsBrain->Update(inputs, CNeuralNet::active);

	if (outputs.size() < CParams::iNumOutputs)
		return false;

	return true;
}

void AutomatedPlayer::Input()
{
	GameManager::Get()->GetLogger().AddDebugLog(std::format("Player {}", GameManager::Get()->GetAIController()->GetCurrentPlayerNum()), false);

	for (int i = 0; i < outputs.size(); ++i)
	{
		bool output = false;
		double oval = outputs[i];

		std::string move = "";
		switch (i)
		{
		case Keys::LEFT_KEY:
			move = "left";
			break;
		case Keys::RIGHT_KEY:
			move = "rght";
			break;
		case Keys::UP_KEY:
			move = "look";
			break;
		case Keys::DOWN_KEY:
			move = "down";
			break;
		case Keys::RUN_KEY:
			move = "run";
			break;
		case Keys::JUMP_KEY:
			move = "jump";
			break;
		case Keys::SJUMP_KEY:
			move = "sJmp";
			break;
		default:
			break;
		}

		//clamp output
		if (oval <= 0.1) output = false;
		else if (oval >= 0.9)
			output = true;
		else output = false;

		GameManager::Get()->GetLogger().AddDebugLog(std::format("{} = {} = {}", move, oval, output), false);
		GameManager::Get()->GetLogger().AddDebugLog("\t", false);

		//store output
		GameManager::Get()->GetInputManager().SetKeyState(i, output);
	}

	GameManager::Get()->GetLogger().AddDebugLog("");
}