#include "../Controller/AIController.h"
#include <format>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"

namespace
{
	std::string OutputDebugMessage(bool didNotMove, bool movedLeft, bool movedRight, bool plyDeath, bool lvlFin)
	{
		std::string msg = "new player has been assigned because previous player ";

		if (plyDeath)
			msg += std::format("because of player death", CParams::iNumTicks);

		if (lvlFin)
			msg += std::format("because of player's level completion", CParams::iNumTicks);

		if (didNotMove)
		{
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
		}

		return msg;

	}
}

AIController::AIController()
	: m_timer(CParams::iNumTicks)
{
	m_AnnView = std::make_unique<ANNView>();

	if (GameConstants::Automated)
	{
		Point startPos = GameManager::Get()->GetCollisionMgr()->GetTile(2, 11)->GetPosition();
		for (int i = 0; i< CParams::iNumPlayers; ++i)
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
	}

	m_inputs.resize(255);
}

bool AIController::Update(float deltaTime)
{
	m_timer.Update(deltaTime);

	auto currPly = GetCurrentPlayer();

	auto currPosX = currPly->GetPosition().x;
	auto prevPosX = currPly->GetPosition().x;

	bool didNotMove = (currPosX == prevPosX);
	bool movedLeft = (currPosX < currPly->GetInitialPosition().x);
	bool movedRight = (currPosX > currPly->GetInitialPosition().x);
	bool plyDeath = !currPly->GetIsAlive();
	bool lvlFin = currPly->GetGoalHit();

	if ((didNotMove || movedLeft) && m_timer.CheckEnd()
		|| plyDeath
		|| lvlFin)
	{
		m_currPlayer++;
		m_timer.ResetTime();
		if (m_currPlayer < m_players.size())
		{
			std::cout << OutputDebugMessage(didNotMove, movedLeft, movedRight, plyDeath, lvlFin);
			GameManager::Get()->ChangePlayer(GetCurrentPlayer());
			GameManager::Get()->GetPlayer()->Reset();
		}
	}
	else if (currPosX > prevPosX)
	{
		std::cout << "current player has moved further right\n";
		m_timer.ResetTime();
	}

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
		for (int swp = 0; swp < m_players.size(); ++swp)
		{
			GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player: {}", swp), false);
			EndOfRunCalculation(m_players[swp].get());
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
		GameManager::Get()->GetPlayer()->Reset();

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
	if (col == sf::Color::Red) return 0.0f;
	if (col == sf::Color::Transparent) return 0.2f;
	if (col == sf::Color::White) return 0.4f;
	if (col == sf::Color::Green) return 0.6f;
	if (col == sf::Color::Yellow) return 0.8f;
	if (col == sf::Color::Black) return 1.0f;

	return -1.0f;
}

void AIController::EndOfRunCalculation(AutomatedPlayer* ply)
{
	float percent = ((ply->GetPosition().x - ply->GetInitialPosition().x) / GameConstants::RightMost) * 100;;
	GameManager::Get()->GetLogger().AddExperimentLog(std::format("Player moved by {}%!", percent));
	ply->UpdateFitness(percent);
}
