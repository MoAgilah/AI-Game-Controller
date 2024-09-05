#include "PlayerState.h"
#include <format>
#include <iostream>
#include "../Collisions/AABB.h"
#include "../Game/GameManager.h"

void GroundedState::Initialise()
{
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::IDLE);
}

void GroundedState::Resume()
{
	Player* player = GetPlayer();
	auto animSpr = GetPlayer()->GetAnimSpr();

	if (player->GetXVelocity() == 0)
	{
		animSpr->ChangeAnim(MarioAnims::IDLE);
	}
	else
	{
		switch (player->GetPhysicsController()->GetXVelocityType())
		{
		case XVelocity::walking:
			animSpr->UpdateAnimSpeed(0.5f);
			animSpr->ChangeAnim(MarioAnims::MOVING);
			break;
		case XVelocity::running:
			animSpr->UpdateAnimSpeed(0.75f);
			animSpr->ChangeAnim(MarioAnims::MOVING);
			break;
		case XVelocity::sprinting:
			animSpr->UpdateAnimSpeed(0.5f);
			animSpr->ChangeAnim(MarioAnims::RUNNING);
			break;
		}
	}

	PlayerState::Resume();
}

void GroundedState::ProcessInputs()
{
	Player* player = GetPlayer();
	AnimatedSprite* animSpr = player->GetAnimSpr();
	PhysicsController* physicCtrl = player->GetPhysicsController();

	auto& keyStates = player->GetKeyStates();
	if (keyStates[Keys::LEFT_KEY])
	{
		if (player->GetDirection())
		{
			player->SetDirection(false);
			if (player->GetXVelocity() > physicCtrl->GetXAcceleration())
			{
				player->SetXVelocity(player->GetXVelocity() / 2);
				player->DecrementXVelocity(0.03125);
				animSpr->ChangeAnim(MarioAnims::SKID);
				m_turningAround = true;
				m_turnTime = 0.2f;
			}
		}

		if (!m_turningAround)
		{
			switch (player->GetPhysicsController()->GetXVelocityType())
			{
			case XVelocity::walking:
				animSpr->UpdateAnimSpeed(0.5f);
				animSpr->ChangeAnim(MarioAnims::MOVING);
				break;
			case XVelocity::running:
				animSpr->UpdateAnimSpeed(0.75f);
				animSpr->ChangeAnim(MarioAnims::MOVING);
				break;
			case XVelocity::sprinting:
				animSpr->UpdateAnimSpeed(0.5f);
				animSpr->ChangeAnim(MarioAnims::RUNNING);
				break;
			}
			player->DecrementXVelocity(physicCtrl->GetXAcceleration());
		}
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		if (!player->GetDirection())
		{
			player->SetDirection(true);
			if (player->GetXVelocity() < -physicCtrl->GetXAcceleration())
			{
				player->SetXVelocity(player->GetXVelocity() / 2);
				player->IncrementXVelocity(0.03125);
				animSpr->ChangeAnim(MarioAnims::SKID);
				m_turningAround = true;
				m_turnTime = 0.2f;
			}
		}

		if (!m_turningAround)
		{
			switch (player->GetPhysicsController()->GetXVelocityType())
			{
			case XVelocity::walking:
				animSpr->UpdateAnimSpeed(0.5f);
				animSpr->ChangeAnim(MarioAnims::MOVING);
				break;
			case XVelocity::running:
				animSpr->UpdateAnimSpeed(0.75f);
				animSpr->ChangeAnim(MarioAnims::MOVING);
				break;
			case XVelocity::sprinting:
				animSpr->UpdateAnimSpeed(0.5f);
				animSpr->ChangeAnim(MarioAnims::RUNNING);
				break;
			}
			player->IncrementXVelocity(physicCtrl->GetXAcceleration());
		}
	}

	if (keyStates[Keys::UP_KEY])
		animSpr->ChangeAnim(MarioAnims::LOOKUP);

	if (keyStates[Keys::RUN_KEY])
	{
		if (physicCtrl->GetXVelocityType() == XVelocity::walking)
			physicCtrl->SetRunning();
	}
	else
	{
		if (physicCtrl->GetXVelocityType() != XVelocity::walking)
			physicCtrl->SetWalking();
	}

	if (keyStates[Keys::JUMP_KEY])
	{
		if (player->GetOnGround() && !player->GetCantJump())
		{
			animSpr->UpdateAnimSpeed(0.5f);
			switch (player->GetPhysicsController()->GetXVelocityType())
			{
			case XVelocity::walking:
				animSpr->ChangeAnim(MarioAnims::JUMP);
				break;
			case XVelocity::running:
				animSpr->ChangeAnim(MarioAnims::JUMP);
				break;
			case XVelocity::sprinting:
				animSpr->ChangeAnim(MarioAnims::RUNJUMP);
				break;
			}

			player->SetAirbourne(true);
			player->SetCantJump(true);
			if (physicCtrl->GetPhysicsType() != PhysicsType::rise)
				physicCtrl->SetAerial();
		}
	}
	else
	{
		player->SetCantJump(false);
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (player->GetOnGround() && !player->GetCantSpinJump())
		{
			animSpr->UpdateAnimSpeed(0.5f);
			animSpr->ChangeAnim(MarioAnims::SPINJUMP);
			player->SetAirbourne(true);
			player->SetCantSpinJump(true);
			if (physicCtrl->GetPhysicsType() != PhysicsType::rise)
				physicCtrl->SetAerial();
		}
	}
	else
	{
		player->SetCantSpinJump(false);
	}
}

void GroundedState::Update(float deltaTime)
{
	if (m_turningAround)
	{
		m_turnTime -= deltaTime;
		if (m_turnTime < 0)
			m_turningAround = false;
	}
}

void AirborneState::Initialise()
{
}

void AirborneState::ProcessInputs()
{
	Player* player = GetPlayer();
	PhysicsController* physicCtrl = player->GetPhysicsController();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[Keys::LEFT_KEY])
	{
		if (player->GetDirection())
			player->SetDirection(false);

		player->DecrementXVelocity(physicCtrl->GetXAcceleration());
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		 if (!player->GetDirection())
			player->SetDirection(true);

		 player->IncrementXVelocity(physicCtrl->GetXAcceleration());
	}

	if (!keyStates[Keys::JUMP_KEY])
	{
		if (player->GetAirbourne() && player->GetCantJump())
		{
			player->GetAirTimer()->SetTime(0);
		}
	}

	if (!keyStates[Keys::SJUMP_KEY])
	{
		if (player->GetAirbourne() && player->GetCantSpinJump())
		{
			player->GetAirTimer()->SetTime(0);
		}
	}
}

void AirborneState::Update(float deltaTime)
{
}

void CrouchingState::Initialise()
{
	Player* player = GetPlayer();
	player->SetXVelocity(0);
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::CROUCH);
}

void CrouchingState::Resume()
{
	Player* player = GetPlayer();
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::CROUCH);

	player->SetXVelocity(0);

	//get current position
	sf::Vector2f pos = player->GetPosition();

	PlayerState::Resume();
}

void CrouchingState::ProcessInputs()
{
	Player* player = GetPlayer();
	PhysicsController* physicCtrl = player->GetPhysicsController();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[Keys::JUMP_KEY])
	{
		if (!player->GetCantJump())
		{
			player->SetAirbourne(true);
			player->SetCantJump(true);
		}
	}
	else
	{
		player->SetCantJump(false);
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (!player->GetCantSpinJump())
		{
			GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::SPINJUMP);
			player->SetAirbourne(true);
			player->SetCantSpinJump(true);
		}
	}
	else
	{
		player->SetCantSpinJump(false);
	}
}

void CrouchingState::Update(float deltaTime)
{
}

void DieingState::Initialise()
{
	Player* player = GetPlayer();
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::DIE);
	player->SetAirbourne(true);
}

void DieingState::ProcessInputs()
{
	// can't move when dieing
}

void DieingState::Update(float deltaTime)
{
	Player* player = GetPlayer();

	if (player->GetAirbourne())
	{
		player->GetAirTimer()->Update(deltaTime);
		if (player->GetAirTimer()->CheckEnd())
			player->SetAirbourne(false);

		player->DecrementYVelocity(0.625f);
		player->Move(sf::Vector2f(0, player->GetYVelocity() * FPS * deltaTime));
	}
	else
	{
		player->IncrementYVelocity(0.3125f);
		player->Move(sf::Vector2f(0, player->GetYVelocity() * FPS * deltaTime));
		if (!GameManager::GetGameMgr()->GetCamera()->IsInView(player->GetAABB()))
		{
			if (!Automated)
				player->Reset();
		}
	}
}