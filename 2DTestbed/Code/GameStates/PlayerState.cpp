#include "PlayerState.h"
#include <format>
#include <iostream>
#include "../Collisions/AABB.h"
#include "../Game/GameManager.h"

void GroundedState::Initialise()
{
	m_animSpr->ChangeAnim(MarioAnims::IDLE);
}

void GroundedState::Resume()
{
	if (m_player->GetXVelocity() == 0)
		m_animSpr->ChangeAnim(MarioAnims::IDLE);
	else
		UpdateGroundAnimation();

	PlayerState::Resume();
}

void GroundedState::ProcessInputs()
{
	auto& keyStates = m_player->GetKeyStates();
	if (keyStates[Keys::LEFT_KEY])
	{
		if (m_player->GetDirection())
		{
			m_player->SetDirection(false);
			if (m_player->GetXVelocity() > m_physCtrl->GetXAcceleration())
				Slide(m_player->GetDirection());
		}

		if (!m_turningAround)
		{
			UpdateGroundAnimation();
			m_player->DecrementXVelocity(m_physCtrl->GetXAcceleration());
		}
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		if (!m_player->GetDirection())
		{
			m_player->SetDirection(true);
			if (m_player->GetXVelocity() < -m_physCtrl->GetXAcceleration())
				Slide(m_player->GetDirection());
		}

		if (!m_turningAround)
		{
			UpdateGroundAnimation();
			m_player->IncrementXVelocity(m_physCtrl->GetXAcceleration());
		}
	}

	if (keyStates[Keys::UP_KEY])
		m_animSpr->ChangeAnim(MarioAnims::LOOKUP);

	if (keyStates[Keys::RUN_KEY])
	{
		if (m_physCtrl->GetXVelocityType() == XVelocity::walking)
			m_physCtrl->SetRunning();
	}
	else
	{
		if (m_physCtrl->GetXVelocityType() != XVelocity::walking)
			m_physCtrl->SetWalking();
	}

	if (keyStates[Keys::JUMP_KEY])
	{
		if (m_player->GetOnGround() && !m_player->GetCantJump())
		{
			m_animSpr->UpdateAnimSpeed(0.5f);
			switch (m_physCtrl->GetXVelocityType())
			{
			case XVelocity::walking:
				[[fallthrough]];
			case XVelocity::running:
				if (m_animSpr->GetCurrentAnim() != MarioAnims::JUMP)
					m_animSpr->ChangeAnim(MarioAnims::JUMP);
				break;
			case XVelocity::sprinting:
				if (m_animSpr->GetCurrentAnim() != MarioAnims::RUNJUMP)
					m_animSpr->ChangeAnim(MarioAnims::RUNJUMP);
				break;
			}

			m_player->SetAirbourne(true);
			m_player->SetCantJump(true);
			if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
				m_physCtrl->SetAerial();
		}
	}
	else
	{
		m_player->SetCantJump(false);
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (m_player->GetOnGround() && !m_player->GetCantSpinJump())
		{
			m_animSpr->UpdateAnimSpeed(0.5f);
			m_animSpr->ChangeAnim(MarioAnims::SPINJUMP);
			m_player->SetAirbourne(true);
			m_player->SetCantSpinJump(true);
			if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
				m_physCtrl->SetAerial();
		}
	}
	else
	{
		m_player->SetCantSpinJump(false);
	}
}

void GroundedState::Update(float deltaTime)
{
	if (m_turningAround)
	{
		m_turnTimer.Update(deltaTime);
		if (m_turnTimer.CheckEnd())
			m_turningAround = false;
	}
}

void GroundedState::UpdateGroundAnimation()
{
	switch (m_physCtrl->GetXVelocityType())
	{
	case XVelocity::walking:
		m_animSpr->UpdateAnimSpeed(0.5f);
		if (m_animSpr->GetCurrentAnim() != MarioAnims::MOVING)
			m_animSpr->ChangeAnim(MarioAnims::MOVING);
		break;
	case XVelocity::running:
		m_animSpr->UpdateAnimSpeed(0.75f);
		if (m_animSpr->GetCurrentAnim() != MarioAnims::MOVING)
			m_animSpr->ChangeAnim(MarioAnims::MOVING);
		break;
	case XVelocity::sprinting:
		m_animSpr->UpdateAnimSpeed(0.5f);
		if (m_animSpr->GetCurrentAnim() != MarioAnims::MOVING)
			m_animSpr->ChangeAnim(MarioAnims::RUNNING);
		break;
	}
}

void GroundedState::Slide(bool dir)
{
	m_player->SetXVelocity(m_player->GetXVelocity() / 2);
	m_player->DecrementXVelocity((dir ? -1.f : 1.f) * 0.3125f);
	m_animSpr->ChangeAnim(MarioAnims::SKID);
	m_turningAround = true;
	m_turnTimer.SetTime(0.2f);
}

void AirborneState::Initialise()
{
	// nothing to update
}

void AirborneState::ProcessInputs()
{
	auto& keyStates = m_player->GetKeyStates();

	if (keyStates[Keys::LEFT_KEY])
	{
		if (m_player->GetDirection())
			m_player->SetDirection(false);

		m_player->DecrementXVelocity(m_physCtrl->GetXAcceleration());
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		 if (!m_player->GetDirection())
			 m_player->SetDirection(true);

		 m_player->IncrementXVelocity(m_physCtrl->GetXAcceleration());
	}

	if (!keyStates[Keys::JUMP_KEY])
	{
		if (m_player->GetAirbourne() && m_player->GetCantJump())
			m_player->GetAirTimer()->SetTime(0);
	}

	if (!keyStates[Keys::SJUMP_KEY])
	{
		if (m_player->GetAirbourne() && m_player->GetCantSpinJump())
			m_player->GetAirTimer()->SetTime(0);
	}
}

void AirborneState::Update(float deltaTime)
{
	// nothing to update
}

void CrouchingState::Initialise()
{
	m_player->SetXVelocity(0);
	m_player->GetAnimSpr()->ChangeAnim(MarioAnims::CROUCH);
}

void CrouchingState::Resume()
{
	m_animSpr->ChangeAnim(MarioAnims::CROUCH);
	m_player->SetXVelocity(0);
	PlayerState::Resume();
}

void CrouchingState::ProcessInputs()
{
	auto& keyStates = m_player->GetKeyStates();

	if (keyStates[Keys::JUMP_KEY])
	{
		if (!m_player->GetCantJump())
			m_player->SetAirbourne(true);
	}
	else
	{
		m_player->SetCantJump(false);
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (!m_player->GetCantSpinJump())
		{
			m_player->GetAnimSpr()->ChangeAnim(MarioAnims::SPINJUMP);
			m_player->SetAirbourne(true);
		}
	}
	else
	{
		m_player->SetCantSpinJump(false);
	}
}

void CrouchingState::Update(float deltaTime)
{
	// nothing to update
}

void DieingState::Initialise()
{
	m_animSpr->ChangeAnim(MarioAnims::DIE);
	m_player->SetAirbourne(true);
}

void DieingState::ProcessInputs()
{
	// can't move when dieing
}

void DieingState::Update(float deltaTime)
{
	if (m_player->GetAirbourne())
	{
		m_player->GetAirTimer()->Update(deltaTime);
		if (m_player->GetAirTimer()->CheckEnd())
			m_player->SetAirbourne(false);

		if (m_physCtrl->GetPhysicsType() != PhysicsType::rise)
			m_physCtrl->SetAerial();

		m_player->DecrementYVelocity(m_physCtrl->GetYAcceleration());
		m_player->Move(sf::Vector2f(0, m_player->GetYVelocity() * GameConstants::FPS * deltaTime));
	}
	else
	{
		if (m_physCtrl->GetPhysicsType() != PhysicsType::drop)
			m_physCtrl->SetAerial();

		m_player->IncrementYVelocity(m_physCtrl->GetYAcceleration());
		m_player->Move(sf::Vector2f(0, m_player->GetYVelocity() * GameConstants::FPS * deltaTime));
		if (GameManager::GetGameMgr()->GetCamera().CheckVerticalBounds(m_player->GetAABB()))
		{
			if (!GameConstants::Automated)
				m_player->Reset();
		}
	}
}