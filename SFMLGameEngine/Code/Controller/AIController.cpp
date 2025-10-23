#include "../Controller/AIController.h"
#include <format>
#include <algorithm>
#include <cmath>
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

	// create the phenotypes
	std::vector<CNeuralNet*> pBrains = m_pop->CreatePhenotypes();

	// assign the phenotypes
	for (int i = 0; i < CParams::iNumPlayers; i++)
		m_players[i]->InsertNewBrain(pBrains[i]);

	// init per-player trackers for the first player
	m_playerStalled.assign(CParams::iNumPlayers, false);
	m_prevPosX = m_players[0]->GetPosition().x;
	m_prevPosY = m_players[0]->GetPosition().y;
	m_inactiveFrames = 0;

	// ---- shaping trackers ----
	m_rightwardPixels.assign(CParams::iNumPlayers, 0.f);
	m_crouchFrames.assign(CParams::iNumPlayers, 0);

	m_superFrames.assign(CParams::iNumPlayers, 0);
	m_startedSuper.assign(CParams::iNumPlayers, false);
	m_framesBoxAhead.assign(CParams::iNumPlayers, 0);
	m_framesTriedBox.assign(CParams::iNumPlayers, 0);

	// seed "startedSuper" for the first current player
	if (!m_players.empty())
		m_startedSuper[0] = m_players[0]->GetIsSuper();
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
	const float EPS_X = 1.5f;        // horizontal deadzone (pixels)
	const float EPS_Y = 1.0f;        // vertical deadzone (pixels)
	const float MIN_SPEEDX = 10.0f;  // min horizontal speed to count as moving

	// use player's horizontal velocity if available
	const float vx = currPly->GetXVelocity();

	const bool movedX = (std::fabs(dx) > EPS_X);
	const bool movedY = (std::fabs(dy) > EPS_Y);      // jumping/falling counts
	const bool movedV = (std::fabs(vx) > MIN_SPEEDX); // running counts

	const bool movedRight = (dx > EPS_X);
	const bool movedLeft = (dx < -EPS_X);
	const bool didNotMove = !(movedX || movedY || movedV);

	// ===== Shaping accumulation (per-frame) =====
	{
		const int idx = m_currPlayer;

		// (a) accumulate rightward-only movement (dense reward toward goal)
		if (dx > 0.f && idx < (int)m_rightwardPixels.size())
			m_rightwardPixels[idx] += dx;

		// (b) penalize crouch time
		if (currPly->GetIsCrouched() && idx < (int)m_crouchFrames.size())
			++m_crouchFrames[idx];

		// (c) time spent with protective power-up (Super)
		if (currPly->GetIsSuper() && idx < (int)m_superFrames.size())
			++m_superFrames[idx];

		// (d) “box reachable” detection using ANN mini-map tiles.
		//     With the **current mismatch**, ANNView paints boxes GREEN.
		const auto& tiles = m_AnnView->GetVecView(); // array<Tile,255>
		const sf::FloatRect plyBox = currPly->GetAABB()->GetRect().getGlobalBounds();

		const float reachX = 48.f;   // ~3 tiles ahead
		const float reachY = 24.f;   // ~1.5 tiles above
		sf::FloatRect reachZone(
			plyBox.left + plyBox.width,   // start at front
			plyBox.top - reachY,          // a bit above head
			reachX,                       // ahead
			plyBox.height + reachY        // down to feet
		);

		bool boxAhead = false;
		for (const auto& t : tiles)
		{
			// ANNView paints boxes GREEN (given current mismatch)
			if (t->GetRect().getFillColor() == sf::Color::Green &&
				t->GetRect().getGlobalBounds().intersects(reachZone))
			{
				boxAhead = true;
				break;
			}
		}

		if (boxAhead && idx < (int)m_framesBoxAhead.size())
		{
			++m_framesBoxAhead[idx];

			// count an "attempt" if we're moving upward (jumping to bonk)
			if (currPly->GetYVelocity() < -1.f && idx < (int)m_framesTriedBox.size())
				++m_framesTriedBox[idx];
		}
	}
	// ===== end shaping accumulation =====

	// inactivity accumulation (~20s window)
	if (didNotMove) ++m_inactiveFrames; else m_inactiveFrames = 0;
	const bool timedOutForInactivity = (m_inactiveFrames > INACTIVITY_LIMIT_FRAMES);

	// compute end-of-run reasons BEFORE we switch players (for debug message)
	const bool plyDeath = !currPly->GetIsAlive();
	const bool lvlFin = currPly->GetGoalHit();

	if (((didNotMove || movedLeft) && m_timer.CheckEnd())
		|| timedOutForInactivity
		|| plyDeath
		|| lvlFin)
	{
		{
			// treat "timed out for inactivity" as "did not move" for the message
			const bool noMoveForMessage = didNotMove || timedOutForInactivity;
			const std::string dbg = OutputDebugMessage(noMoveForMessage, movedLeft, movedRight, plyDeath, lvlFin);


			if (!dbg.empty())
			{
				std::cout << dbg << std::endl;
				GameManager::Get()->GetLogger().AddExperimentLog(dbg);
			}
		}

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

			// also reset shaping counters for the new current player
			m_startedSuper[m_currPlayer] = GetCurrentPlayer()->GetIsSuper();
			m_rightwardPixels[m_currPlayer] = 0.f;
			m_crouchFrames[m_currPlayer] = 0;
			m_superFrames[m_currPlayer] = 0;
			m_framesBoxAhead[m_currPlayer] = 0;
			m_framesTriedBox[m_currPlayer] = 0;
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
			// error in processing the neural net
			std::cout << "Wrong amount of NN inputs!" << std::endl;
			return false;
		}
	}

	//We have completed another generation so now we need to run the GA
	if (m_currPlayer == m_players.size())
	{
		// add up each player's fitness scores (with penalties/bonuses)
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

		//insert the new brains back into the players and reset their state
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

	for (int i = 0; i < (int)tiles.size(); i++)
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
	// Keep the mismatch as requested:
	// Green -> 0.2 (collectable), Yellow -> 0.4 (box)
	if (col == sf::Color::Transparent) return 0.f;  // empty square
	if (col == sf::Color::Green)       return 0.2f; // collectable
	if (col == sf::Color::Yellow)      return 0.4f; // interactable box
	if (col == sf::Color::Red)         return 0.6f; // itself
	if (col == sf::Color::White)       return 0.8f; // "ground" / walkable
	if (col == sf::Color::Black)       return 1.0f; // enemy

	return -1.0f;
}

void AIController::EndOfRunCalculation(AutomatedPlayer* ply, bool stalled)
{
	auto* currPly = ply;

	const float currPosX = currPly->GetPosition().x;
	const float initPosX = currPly->GetInitialPosition().x;

	// base progress -> percent of level traversed
	float percent = 0.f;
	if (currPosX > initPosX)
		percent = ((currPosX - initPosX) / (GameConstants::RightMost - initPosX)) * 100.f;
	else if (currPosX < initPosX)
		percent = ((currPosX - initPosX) / (initPosX - GameConstants::LeftMost)) * 100.f;

	GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player moved by {}%!", percent));

	float fitness = percent;

	const bool died = !currPly->GetIsAlive();
	const bool goal = currPly->GetGoalHit();

	// terminal signals
	if (died)    fitness -= 40.f;   // stronger death penalty (task is "reach right")
	if (goal)    fitness += 200.f;  // big goal bonus
	if (stalled) fitness -= 5.f;    // inactivity penalty

	// index of the just-finished player
	const int idx = std::clamp(m_currPlayer - 1, 0, (int)m_players.size() - 1);

	// grab shaping counters
	const float right = (idx < (int)m_rightwardPixels.size()) ? m_rightwardPixels[idx] : 0.f;
	const int   crou = (idx < (int)m_crouchFrames.size()) ? m_crouchFrames[idx] : 0;

	const bool startedSuper = (idx < (int)m_startedSuper.size()) ? m_startedSuper[idx] : false;
	const int  superFrames = (idx < (int)m_superFrames.size()) ? m_superFrames[idx] : 0;

	const int framesBox = (idx < (int)m_framesBoxAhead.size()) ? m_framesBoxAhead[idx] : 0;
	const int framesTry = (idx < (int)m_framesTriedBox.size()) ? m_framesTriedBox[idx] : 0;

	// --- dense shaping toward "run right" and away from crouch local min ---
	fitness += 0.02f * right;               // a few points per screen of rightward travel
	fitness -= 0.01f * (float)crou;

	// --- power-up shaping (collect protection to avoid deaths) ---
	if (!startedSuper && superFrames > 0) fitness += 40.f;     // got protected at least once
	fitness += 0.02f * (float)superFrames;                     // staying protected is slightly good
	if (died && superFrames == 0) fitness -= 10.f;             // died without ever being protected

	// --- “don’t ignore boxes” shaping ---
	fitness += 0.10f * (float)framesTry;                        // tried to bonk when reachable
	if (framesBox > framesTry)                                  // missed opportunities
		fitness -= 0.05f * (float)(framesBox - framesTry);

	GameManager::Get()->GetLogger().AddExperimentLog(std::format(
		"Terms -> prog:{:.1f} goal:{} death:{} stalled:{} right:{:.1f} crou:{} superFrames:{} boxSeen:{} boxTry:{}",
		percent, goal, died, stalled, right, crou, superFrames, framesBox, framesTry));

	// write fitness
	ply->UpdateFitness(fitness);

	// clear accumulators (defensive if controller instance is reused)
	if (idx < (int)m_rightwardPixels.size()) m_rightwardPixels[idx] = 0.f;
	if (idx < (int)m_crouchFrames.size())    m_crouchFrames[idx] = 0;
	if (idx < (int)m_superFrames.size())     m_superFrames[idx] = 0;
	if (idx < (int)m_framesBoxAhead.size())  m_framesBoxAhead[idx] = 0;
	if (idx < (int)m_framesTriedBox.size())  m_framesTriedBox[idx] = 0;
}