#include "../Controller/Controller.h"
#include "../GameObjects/Player.h"
#include "../Game/GameManager.h"
#include "../Collisions/CollisionManager.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>

Controller::Controller()
{
	m_AnnView = std::make_unique<ANNView>();

	if (Automated)
	{
		Point startPos = GameManager::GetGameMgr()->GetCollisionMgr()->GetTile(2, 11).GetPosition();
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

bool Controller::Update()
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
			GameManager::GetGameMgr()->ChangePlayer(m_players[m_currPlayer].get());
			GameManager::GetGameMgr()->GetPlayer()->Reset();
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
			GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player: {}", swp), false);
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
			GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player: {} Fitness:{}", i, m_players[i]->Fitness()));
			m_players[i]->InsertNewBrain(pBrains[i]);
			m_players[i]->Reset();
		}

		m_currPlayer = 0;
		GameManager::GetGameMgr()->ChangePlayer(m_players[m_currPlayer].get());
		GameManager::GetGameMgr()->GetPlayer()->Reset();

		m_bestFitness = m_pop->BestEverFitness();

		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Best Fitness : {}", m_bestFitness));
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Current Generation: {}", m_generations));
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(GameManager::GetGameMgr()->GetLogger()->GetTimeStamp());
	}

	return true;
}

Player* Controller::GetCurrentPlayer()
{
	return m_players[m_currPlayer].get();
}

const std::vector<double>& Controller::GetGridInputs()
{
	std::vector<std::shared_ptr<Tile>> tiles = m_AnnView->GetVecView();

	for (int i = 0; i < tiles.size(); i++)
		m_inputs[i] = ColourToInput(tiles[i]->GetRect().getFillColor());

	return m_inputs;
}

const std::vector<double>& Controller::GetFitnessScores() const
{
	std::vector<double> scores;

	for (auto& player : m_players)
		scores.push_back(player->Fitness());

	return scores;
}

double Controller::ColourToInput(sf::Color col)
{
	if (col == sf::Color::Red) return 0.0f;
	if (col == sf::Color::Transparent) return 0.2f;
	if (col == sf::Color::White) return 0.4f;
	if (col == sf::Color::Green) return 0.6f;
	if (col == sf::Color::Yellow) return 0.8f;
	if (col == sf::Color::Black) return 1.0f;

	return -1.0f;
}

void Controller::EndOfRunCalculation(AutomatedPlayer* ply)
{
	float percent = ((ply->GetPosition().x - ply->GetInitialPosition().x) / RightMost) * 100;;
	GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player moved by {}%!", percent));
	ply->UpdateFitness(percent);
}
