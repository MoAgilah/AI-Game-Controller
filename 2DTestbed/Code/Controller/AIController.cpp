#include "../Controller/AIController.h"
#include <format>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"

AIController::AIController()
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

bool AIController::Update()
{
	if (m_players[m_currPlayer]->GetPosition().x > m_players[m_currPlayer]->GetPrevPosition().x)
	{
		m_ticks = 0;
	}
	if (m_ticks++ > CParams::iNumTicks || !m_players[m_currPlayer]->GetIsAlive() || m_players[m_currPlayer]->GetGoalHit())
	{
		m_currPlayer++;
		m_ticks = 0;

		if (m_currPlayer < m_players.size())
		{
			GameManager::Get()->ChangePlayer(m_players[m_currPlayer].get());
			GameManager::Get()->GetPlayer()->Reset();
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
		m_ticks = 0;

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
