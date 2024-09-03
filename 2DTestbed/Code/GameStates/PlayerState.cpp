#include "PlayerState.h"
#include <format>
#include <iostream>
#include "../Collisions/AABB.h"
#include "../Game/GameManager.h"
#include "../Game/TextureManager.h"

void GroundedState::Initialise()
{
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::IDLE);
}

void GroundedState::Resume()
{
	Player* player = GetPlayer();
	auto animSpr = GetPlayer()->GetAnimSpr();

	if (player->GetXVelocity() == 0)
		animSpr->ChangeAnim(MarioAnims::IDLE);
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
		if (!player->GetCantJump())
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
			player->SetOnGround(false);
			player->DecrementYVelocity(physicCtrl->GetYAcceleration());
			player->SetCantJump(true);
			if (physicCtrl->GetPhysicsType() != PhysicsType::rise)
			{
				physicCtrl->SetAerial();
			}
		}
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (!player->GetCantSpinJump())
		{
			animSpr->UpdateAnimSpeed(0.5f);
			animSpr->ChangeAnim(MarioAnims::SPINJUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(physicCtrl->GetYAcceleration());
			player->SetCantSpinJump(true);
			if (physicCtrl->GetPhysicsType() != PhysicsType::rise)
			{
				physicCtrl->SetAerial();
			}
		}
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
			player->SetAirTime(c_maxAirTime);
		}
	}

	if (!keyStates[Keys::SJUMP_KEY])
	{
		if (player->GetAirbourne() && player->GetCantSpinJump())
		{
			player->SetAirTime(c_maxAirTime);
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
			player->SetOnGround(false);
			player->DecrementYVelocity(physicCtrl->GetYAcceleration());
			player->SetCantJump(true);
		}
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (!player->GetCantSpinJump())
		{
			GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::SPINJUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(physicCtrl->GetYAcceleration());
			player->SetCantSpinJump(true);
		}
	}
}

void CrouchingState::Update(float deltaTime)
{
}

void DieingState::Initialise()
{
	Player* player = GetPlayer();
	GetPlayer()->GetAnimSpr()->ChangeAnim(MarioAnims::DIE);
	player->SetAirTime(0.66f);
	player->SetAirbourne(true);
	player->DecrementYVelocity(c_jumpSpeed);
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
		player->IncAirTime(deltaTime);
		if (player->GetAirTime() >= c_maxAirTime)
			player->SetAirbourne(false);

		player->Move(sf::Vector2f(0, player->GetYVelocity() * FPS * deltaTime));
	}
	else
	{
		player->Move(sf::Vector2f(0, -player->GetYVelocity() * FPS * deltaTime));
		if (!GameManager::GetGameMgr()->GetCamera()->IsInView(player->GetAABB()))
		{
			if (!Automated)
				player->Reset();
		}
	}
}