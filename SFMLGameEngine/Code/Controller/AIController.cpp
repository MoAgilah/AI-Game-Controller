#include "../Controller/AIController.h"
#include <format>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"

// ADD at top of file (tunable from one place)
#ifndef INACTIVITY_LIMIT_FRAMES
#define INACTIVITY_LIMIT_FRAMES 12000  // ~20s @ 60 FPS
#endif


namespace
{
	std::string OutputDebugMessage(bool didNotMove, bool movedLeft, bool movedRight, bool plyDeath, bool lvlFin)
	{
		std::string msg = "new player has been assigned because previous player ";

		if (plyDeath)
		{
			msg += std::format("because of player death\n", CParams::iNumTicks);
			return msg;
		}

		if (lvlFin)
		{
			msg += std::format("because of player's level completion\n", CParams::iNumTicks);
			return msg;
		}

		if (!didNotMove)
			return std::string();

		msg += std::format("did not move for {} seconds", CParams::iNumTicks);

		if (movedLeft)
		{
			msg += ", after moving left\n";
		}
		else if (movedRight)
		{
			msg += ", after moving right\n";
		}
		else
		{
			msg += "\n";
		}

		return msg;
	}
}

AIController::AIController()
	: m_timer((float)CParams::iNumTicks)
{
	m_AnnView = std::make_unique<ANNView>();

	m_inputs.resize(255);
}

void AIController::InitAIController()
{
	Point startPos = GameManager::Get()->GetCollisionMgr()->GetTile(2, 11)->GetPosition();
	for (int i = 0; i < CParams::iNumPlayers; ++i)
		m_players.push_back(std::make_shared<AutomatedPlayer>(startPos));

	m_pop = std::make_unique<Cga>(CParams::iNumPlayers,
		CParams::iNumInputs,
		CParams::iNumOutputs,
		10, 10);

	//create the phenotypes
	std::vector<CNeuralNet*> pBrains = m_pop->CreatePhenotypes();

	//assign the phenotypes
	for (int i = 0; i < CParams::iNumPlayers; i++)
		m_players[i]->InsertNewBrain(pBrains[i]);

	// init per-player trackers for the first player
	m_playerStalled.assign(CParams::iNumPlayers, false);
	m_prevPosX = m_players[0]->GetPosition().x;
	m_prevPosY = m_players[0]->GetPosition().y;
	m_inactiveFrames = 0;
}

bool AIController::Update(float deltaTime)
{
	m_timer.Update(deltaTime);

	auto currPly = GetCurrentPlayer();

	const float currPosX = currPly->GetPosition().x;
	const float currPosY = currPly->GetPosition().y;
	const float dx = currPosX - m_prevPosX;
	const float dy = currPosY - m_prevPosY;

	// thresholds tuned to ignore jitter but catch real motion
	const float EPS_X = 1.5f;   // horizontal deadzone (pixels)
	const float EPS_Y = 1.0f;   // vertical deadzone (pixels)
	const float MIN_SPEEDX = 10.0f;  // min horizontal speed to count as moving

	// if available in your player class; else set vx = 0.f
	const float vx = currPly->GetXVelocity();

	const bool movedX = (std::fabs(dx) > EPS_X);
	const bool movedY = (std::fabs(dy) > EPS_Y);      // jumping/falling counts
	const bool movedV = (std::fabs(vx) > MIN_SPEEDX); // running counts

	const bool movedRight = (dx > EPS_X);
	const bool movedLeft = (dx < -EPS_X);
	const bool didNotMove = !(movedX || movedY || movedV);

	// inactivity accumulation (~20s window)
	if (didNotMove) ++m_inactiveFrames; else m_inactiveFrames = 0;
	const bool timedOutForInactivity = (m_inactiveFrames > INACTIVITY_LIMIT_FRAMES);

	if (((didNotMove || movedLeft) && m_timer.CheckEnd())
		|| timedOutForInactivity
		|| !currPly->GetIsAlive()
		|| currPly->GetGoalHit())
	{
		// remember stall for the *current* player
		if (m_currPlayer < static_cast<int>(m_playerStalled.size()))
			m_playerStalled[m_currPlayer] = timedOutForInactivity;

		m_currPlayer++;
		m_timer.ResetTime();

		if (m_currPlayer < static_cast<int>(m_players.size()))
		{
			GameManager::Get()->ChangePlayer(GetCurrentPlayer());
			GameManager::Get()->GetPlayer()->Reset();

			// reset trackers for the new current player
			m_prevPosX = GetCurrentPlayer()->GetPosition().x;
			m_prevPosY = GetCurrentPlayer()->GetPosition().y;
			m_inactiveFrames = 0;
		}
	}
	else if (movedRight)
	{
		// made forward progress -> reset dwell timer
		m_timer.ResetTime();
	}

	// update previous position AFTER decisions
	m_prevPosX = currPosX;
	m_prevPosY = currPosY;

	if (m_currPlayer < m_players.size())
	{
		if (!m_players[m_currPlayer]->UpdateANN())
		{
			//error in processing the neural net
			std::cout << "Wrong amount of NN inputs!" << std::endl;

			return false;
		}
	}

	//We have completed another generation so now we need to run the GA
	if (m_currPlayer == m_players.size())
	{
		//first add up each players fitness scores. (remember for this task
		//there are many different sorts of penalties the players may incur
		//and each one has a coefficient)
		for (int swp = 0; swp < static_cast<int>(m_players.size()); ++swp)
		{
			GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player: {}", swp), false);
			const bool stalled = (swp < static_cast<int>(m_playerStalled.size())) ? m_playerStalled[swp] : false;
			EndOfRunCalculation(m_players[swp].get(), stalled);
		}

		//increment the generation counter
		++m_generations;

		//reset cycles
		m_timer.ResetTime();

		//perform an epoch and grab the new brains
		std::vector<CNeuralNet*> pBrains = m_pop->Epoch(GetFitnessScores());

		//insert the new  brains back into the players and reset their
		//state
		for (int i = 0; i < CParams::iNumPlayers; ++i)
		{
			GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player: {} Fitness:{}", i, m_players[i]->Fitness()));
			m_players[i]->InsertNewBrain(pBrains[i]);
			m_players[i]->Reset();
		}

		m_currPlayer = 0;
		GameManager::Get()->ChangePlayer(m_players[m_currPlayer].get());
		m_prevPosX = m_players[0]->GetPosition().x;
		m_prevPosY = m_players[0]->GetPosition().y;
		m_inactiveFrames = 0;

		// clear stall flags for next generation
		std::fill(m_playerStalled.begin(), m_playerStalled.end(), false);

		m_bestFitness = m_pop->BestEverFitness();

		GameManager::Get()->GetLogger().AddExperimentLog(std::format("Best Fitness : {}", m_bestFitness));
		GameManager::Get()->GetLogger().AddExperimentLog(std::format("Current Generation: {}", m_generations));
		GameManager::Get()->GetLogger().AddExperimentLog(GameManager::Get()->GetLogger().GetTimeStamp());
	}

	return true;
}

Player* AIController::GetCurrentPlayer()
{
	return m_players[m_currPlayer].get();
}

const std::vector<double>& AIController::GetGridInputs()
{
	const std::array<std::shared_ptr<Tile>, 255>& tiles = m_AnnView->GetVecView();

	for (int i = 0; i < tiles.size(); i++)
		m_inputs[i] = ColourToInput(tiles[i]->GetRect().getFillColor());

	return m_inputs;
}

std::vector<double> AIController::GetFitnessScores() const
{
	std::vector<double> scores;

	for (auto& player : m_players)
		scores.push_back(player->Fitness());

	return scores;
}

double AIController::ColourToInput(sf::Color col)
{
	if (col == sf::Color::Transparent) return 0.f;
	if (col == sf::Color::Green) return 0.2f;
	if (col == sf::Color::Yellow) return 0.4f;
	if (col == sf::Color::Red) return 0.6f;
	if (col == sf::Color::White) return 0.8f;
	if (col == sf::Color::Black) return 1.0f;

	return -1.0f;
}

void AIController::EndOfRunCalculation(AutomatedPlayer* ply, bool stalled)
{
	auto currPly = ply;

	const float currPosX = currPly->GetPosition().x;
	const float initPosX = currPly->GetInitialPosition().x;

	float percent = 0.f;

	if (currPosX > initPosX)
	{
		// normalize to ~0..100 across level span
		percent = ((currPosX - initPosX) / (GameConstants::RightMost - initPosX)) * 100.f;
	}
	else if (currPosX < initPosX)
	{
		// small negative if moved left
		percent = ((currPosX - initPosX) / (initPosX - GameConstants::LeftMost)) * 100.f;
	}

	GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player moved by {}%!", percent));

	// shaped terminal fitness (minimal, safe)
	float fitness = percent;
	if (!currPly->GetIsAlive())  fitness -= 20.f;   // death penalty
	if (currPly->GetGoalHit())   fitness += 200.f;  // goal bonus
	if (stalled)                 fitness -= 5.f;    // inactivity penalty

	GameManager::Get()->GetLogger().AddExperimentLog(std::format(
		"Fitness terms -> progress:{:.2f}, goal:{}, death:{}, stalled:{}",
		percent, currPly->GetGoalHit(), !currPly->GetIsAlive(), stalled));

	ply->UpdateFitness(fitness);
}
