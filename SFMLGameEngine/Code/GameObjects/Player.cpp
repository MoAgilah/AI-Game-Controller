#include "Player.h"
#include <format>
#include <iostream>
#include "../NEAT/phenotype.h"
#include "../Game/GameManager.h"
#include "../GameStates/PlayerState.h"

Player::Player(const sf::Vector2f& pos)
	: DynamicObject(new AnimatedSprite(TexID::Mario, 14, 4, GameConstants::FPS, false, 0.5f), sf::Vector2f(9, 16)), m_airTimer(0.4f), m_invulTimer(1)
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
	PlayerState::s_frameStep = GameConstants::FPS * deltaTime;

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
							DecrementXVelocity(physCtrl->GetXAcceleration() * PlayerState::s_frameStep);

						if (GetSlideRight())
							IncrementXVelocity(physCtrl->GetXAcceleration() * PlayerState::s_frameStep);

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
				{
					m_stateMgr.PushState(new AirborneState(this));
					// reset jump timer exactly once on entering Airborne
					m_airTimer.SetTime(physCtrl->GetAirTime());
				}

				m_airTimer.Update(deltaTime);
				if (m_airTimer.CheckEnd())
				{
					SetAirbourne(false);
					m_airTimer.ResetTime();
				}
				else
				{
					if (m_airTimer.GetTime() < physCtrl->GetAirTime() * 0.6f)
						DecrementYVelocity(physCtrl->GetYAcceleration() * PlayerState::s_frameStep);
				}
			}
			else
			{
				IncrementYVelocity(GameConstants::Gravity * PlayerState::s_frameStep);
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
			Move(sf::Vector2f(GetXVelocity() * PlayerState::s_frameStep, 0));
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
			Move(sf::Vector2f(0, GetYVelocity() * PlayerState::s_frameStep));
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
	m_stateMgr.ProcessInputs();
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
	auto ai = GameManager::Get()->GetAIController();
	std::vector<double> inputs = ai->GetGridInputs();
	outputs = m_itsBrain->Update(inputs, CNeuralNet::active);
	return !outputs.empty();
}

void AutomatedPlayer::Input()
{
	auto* gm = GameManager::Get();
	gm->GetLogger().AddDebugLog(std::format("Player {}", gm->GetAIController()->GetCurrentPlayerNum()), false);

	// Map ANN output index -> actual input keys (keep this aligned with your ANN outputs)
	static const std::array<int, 7> keyOrder = {
		Keys::LEFT_KEY,
		Keys::RIGHT_KEY,
		Keys::UP_KEY,
		Keys::DOWN_KEY,
		Keys::RUN_KEY,
		Keys::JUMP_KEY,
		Keys::SJUMP_KEY
	};

	// Labels for one-line debug logging
	static const std::array<const char*, 7> moveNames = {
		"left", "rght", "look", "down", "run", "jump", "sJmp"
	};

	// Persisted hysteresis state per ANN-controlled key
	static std::array<bool, 7> last = { false, false, false, false, false, false, false };

	// Base hysteresis band around 0.5
	constexpr double onDefault = 0.55;  // press if >= this
	constexpr double offDefault = 0.45;  // release if <= this

	// Convenience handles for crouch gating
	const bool onGround = gm->GetPlayer()->GetOnGround();
	const bool onSlope = gm->GetPlayer()->GetOnSlope();
	const float vx = gm->GetPlayer()->GetXVelocity();

	for (size_t i = 0; i < keyOrder.size() && i < outputs.size(); ++i)
	{
		const double oval = outputs[i];

		// Per-key priors chosen by KEY (order-independent thresholds)
		double onTh = onDefault;
		double offTh = offDefault;

		switch (keyOrder[i]) {
		case Keys::RIGHT_KEY:
		case Keys::RUN_KEY:
			onTh = 0.50;  offTh = 0.40;  // nudge to keep moving right
			break;
		case Keys::LEFT_KEY:
			onTh = 0.65;  offTh = 0.50;  // discourage going left
			break;
		case Keys::DOWN_KEY:
			onTh = 0.75;  offTh = 0.52;  // make crouch rarer & release sooner
			break;
		default:
			break;
		}

		// Hysteresis
		bool state = last[i];
		if (!state && oval >= onTh)       state = true;
		else if (state && oval <= offTh)  state = false;

		// Light action gating for crouch: don't crouch while airborne/slope/high speed
		if (keyOrder[i] == Keys::DOWN_KEY) {
			if (!onGround || onSlope || std::fabs(vx) > 5.f) {
				state = false;
			}
		}

		// One-shot diagnostic: which output index actually toggles crouch ON?
		const bool pressedThisFrame = (!last[i] && state);
		if (keyOrder[i] == Keys::DOWN_KEY && pressedThisFrame) {
			gm->GetLogger().AddExperimentLog(
				std::format("[ANN] DOWN toggled ON by output index {}", i)
			);
		}

		last[i] = state;

		// Per-frame debug row (key = value = state)
		gm->GetLogger().AddDebugLog(std::format("{} = {} = {}", moveNames[i], oval, state), false);
		gm->GetLogger().AddDebugLog("\t", false);

		// Apply to the actual mapped key
		gm->GetInputManager().SetKeyState(keyOrder[i], state);
	}

	gm->GetLogger().AddDebugLog(""); // end the line
}

