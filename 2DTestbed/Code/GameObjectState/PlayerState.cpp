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
		player->SetXVelocity(c_moveSpeed);
	}

	if (keyStates[UP_KEY])
		player->GetAnimSpr()->ChangeAnim(LOOKUP);

	if (keyStates[JUMP_KEY])
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

	if (keyStates[SPINJUMP])
	{
		if (!player->GetCantSpinJump())
		{
			player->GetAnimSpr()->ChangeAnim(SPINJUMP);
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

		player->SetXVelocity(c_moveSpeed);
	}

	if (!keyStates[JUMP_KEY])
	{
		if (player->GetAirbourne() && player->GetCantJump())
		{
			if (!player->GetIsCrouched())
				player->GetAnimSpr()->ChangeAnim(FALL);
			player->SetAirTime(c_maxAirTime);
		}
	}

	if (!keyStates[SPINJUMP])
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
	player->GetAnimSpr()->ChangeAnim(CROUCH);
}

void CrouchingState::Resume()
{
	Player* player = GetPlayer();
	player->GetAnimSpr()->ChangeAnim(CROUCH);

	player->SetXVelocity(0);

	//get current position
	sf::Vector2f pos = player->GetPosition();

	if (player->GetIsSuper())
	{
		//adjust bbox position
		if (player->GetDirection())
			player->GetBBox()->Update(sf::Vector2f(player->GetPosition().x - 1.f, player->GetPosition().y + 22.f));
		else
			player->GetBBox()->Update(sf::Vector2f(player->GetPosition().x + 1.f, player->GetPosition().y + 22.f));
	}
	else
	{
		//adjust bbox position
		if (player->GetDirection())
			player->GetBBox()->Update(sf::Vector2f(player->GetPosition().x - 2.f, player->GetPosition().y + 12.f));
		else
			player->GetBBox()->Update(sf::Vector2f(player->GetPosition().x + 2.f, player->GetPosition().y + 12.f));
	}

	PlayerState::Resume();
}

void CrouchingState::ProcessInputs()
{
	Player* player = GetPlayer();
	auto& keyStates = player->GetKeyStates();

	if (keyStates[JUMP_KEY])
	{
		if (!player->GetCantJump())
		{
			player->SetAirbourne(true);
			player->SetOnGround(false);
			player->DecrementYVelocity(c_jumpSpeed);
			player->SetCantJump(true);
		}
	}

	if (keyStates[SPINJUMP])
	{
		if (!player->GetCantSpinJump())
		{
			player->GetAnimSpr()->ChangeAnim(SPINJUMP);
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
	player->GetAnimSpr()->ChangeAnim(DIE);
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
		if (!Game::GetGameMgr()->GetCamera()->OnScreen(player))
		{
			if (!Automated)
				player->Reset();
		}
	}
}