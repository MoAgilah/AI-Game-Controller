#include "PlayerState.h"
#include "../Game/Game.h"
#include "../Game/TextureManager.h"
#include "../Collisions/BoundingBox.h"

void GroundedState::Initialise()
{
	GetPlayer()->GetAnimSpr()->ChangeAnim(IDLE);
}

void GroundedState::Resume()
{
	Player* player = GetPlayer();

	if (player->GetXVelocity() == 0)
		player->GetAnimSpr()->ChangeAnim(IDLE);
	else
		player->GetAnimSpr()->ChangeAnim(MOVING);

	PlayerState::Resume();
}

void GroundedState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[LEFT_KEY])
	{
		if (player->GetDirection())
			player->SetDirection(false);

		player->GetAnimSpr()->ChangeAnim(MOVING);
		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[RIGHT_KEY])
	{
		if (!player->GetDirection())
			player->SetDirection(true);

		player->GetAnimSpr()->ChangeAnim(MOVING);
		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[UP_KEY])
		player->GetAnimSpr()->ChangeAnim(LOOKUP);

	if (keyStates[SPACE_KEY])
	{
		if (!player->GetCantJump())
		{
			player->GetAnimSpr()->ChangeAnim(JUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}

	if (keyStates[RCRTL_KEY])
	{
		if (!player->GetCantJump())
		{
			player->GetAnimSpr()->ChangeAnim(SPINJUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}
}

void GroundedState::Update(float deltaTime)
{
	ProcessInputs();
}

void AirborneState::Initialise()
{
}

void AirborneState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[LEFT_KEY])
	{
		if (player->GetDirection())
			player->SetDirection(false);

		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[RIGHT_KEY])
	{
		if (!player->GetDirection())
			player->SetDirection(true);

		player->SetXVelocity(-c_moveSpeed);
	}

	if (keyStates[SPACE_KEY])
		player->DecrementYVelocity(c_jumpSpeed);

	if (keyStates[RCRTL_KEY])
		player->DecrementYVelocity(c_jumpSpeed);
}

void AirborneState::Update(float deltaTime)
{
	ProcessInputs();
}

void CrouchingState::Initialise()
{
	GetPlayer()->GetAnimSpr()->ChangeAnim(CROUCH);
}

void CrouchingState::Resume()
{
	GetPlayer()->GetAnimSpr()->ChangeAnim(CROUCH);
	PlayerState::Resume();
}

void CrouchingState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[SPACE_KEY])
	{
		if (!player->GetCantJump())
		{
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}

	if (keyStates[RCRTL_KEY])
	{
		if (!player->GetCantJump())
		{
			player->GetAnimSpr()->ChangeAnim(SPINJUMP);
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}
}

void CrouchingState::Update(float deltaTime)
{
	ProcessInputs();
}

void DieingState::Initialise()
{
	Player* player = GetPlayer();
	player->GetAnimSpr()->ChangeAnim(SPINJUMP);
	player->SetAirTime(0.33f);
	player->SetAirbourne(true);
}

void DieingState::ProcessInputs()
{
	// can't move when dieing
}

void DieingState::Update(float deltaTime)
{
	ProcessInputs();

	Player* player = GetPlayer();

	if (player->GetAirbourne())
	{
		player->DecrementYVelocity(c_jumpSpeed);
		player->IncAirTime(deltaTime);
		if (player->GetAirTime() >= c_maxAirTime)
			player->SetAirbourne(false);
	}
	else
	{

		if (!Game::GetGameMgr()->GetCamera()->OnScreen(player))
		{
			if (!Automated)
			{
				player->Reset();
			}
		}
	}
}