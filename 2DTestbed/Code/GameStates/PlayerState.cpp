#include "PlayerState.h"
#include "../Game/GameManager.h"
#include "../Game/TextureManager.h"
#include "../Collisions/AABB.h"

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
		animSpr->ChangeAnim(MarioAnims::MOVING);

	PlayerState::Resume();
}

void GroundedState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();
	auto animSpr = GetPlayer()->GetAnimSpr();

	if (keyStates[Keys::LEFT_KEY])
	{
		if (player->GetDirection())
			player->SetDirection(false);

		animSpr->ChangeAnim(MarioAnims::MOVING);
		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		if (!player->GetDirection())
			player->SetDirection(true);

		animSpr->ChangeAnim(MarioAnims::MOVING);
		player->SetXVelocity(c_moveSpeed);
	}

	if (keyStates[Keys::UP_KEY])
		animSpr->ChangeAnim(MarioAnims::LOOKUP);

	if (keyStates[Keys::JUMP_KEY])
	{
		if (!player->GetCantJump())
		{
			animSpr->ChangeAnim(MarioAnims::JUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}

	if (keyStates[Keys::SJUMP_KEY])
	{
		if (!player->GetCantSpinJump())
		{
			animSpr->ChangeAnim(MarioAnims::SPINJUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantSpinJump(true);
		}
	}
}

void GroundedState::Update(float deltaTime)
{
}

void AirborneState::Initialise()
{
}

void AirborneState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[Keys::LEFT_KEY])
	{
		if (player->GetDirection())
			player->SetDirection(false);

		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[Keys::RIGHT_KEY])
	{
		 if (!player->GetDirection())
			player->SetDirection(true);

		player->SetXVelocity(c_moveSpeed);
	}

	if (!keyStates[Keys::JUMP_KEY])
	{
		if (player->GetAirbourne() && player->GetCantJump())
		{
			if (!player->GetIsCrouched())
				GetPlayer()->GetAnimSpr()->ChangeAnim(FALL);
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
	auto& keyStates = player->GetKeyStates();

	if (keyStates[Keys::JUMP_KEY])
	{
		if (!player->GetCantJump())
		{
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
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
			player->DecrementYVelocity(c_jumpSpeed);
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