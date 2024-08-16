#include "../Controller/Controller.h"
#include "../GameObjects/Player.h"

#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>

Controller::Controller()
{
	m_AnnView = std::make_unique<ANNView>();

	m_numPlayers = CParams::iNumPlayers;
	m_currPlayer = 0;
	m_bestFitness = 0;

	if (Automated)
	{
		//let's create the players
		for (int i = 0; i< m_numPlayers; ++i)
		{
			m_players.push_back(std::make_shared<AutomatedPlayer>(sf::Vector2f(75, 454)));
		}

		m_pop = std::make_unique<Cga>(CParams::iNumPlayers,
			CParams::iNumInputs,
			CParams::iNumOutputs,
			10, 10);

		//create the phenotypes
		std::vector<CNeuralNet*> pBrains = m_pop->CreatePhenotypes();

		//assign the phenotypes
		for (int i = 0; i< m_numPlayers; i++)
		{
			m_players[i]->InsertNewBrain(pBrains[i]);
		}
	}

	m_ticks = 0;
	m_generations = 0;

	m_inputs.resize(255);
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

std::vector<double> Controller::GetGridInputs()
{
	std::vector<std::shared_ptr<Tile>> tiles = m_AnnView->GetVecView();

	for (int i = 0; i < tiles.size(); i++)
	{
		m_inputs[i] = ColourToInput(tiles[i]->GetRect().getFillColor());
	}

	return m_inputs;
}

Player* Controller::GetCurrentPlayer()
{
	return m_players[m_currPlayer].get();
}

bool Controller::Update()
{
	//if havent moved in 2000 ticks
	if ((m_players[m_currPlayer]->GetPosition().x == m_players[m_currPlayer]->GetPrevPosition().x && m_ticks++ > CParams::iNumTicks) ||
		//if been killed
		m_players[m_currPlayer]->GetIsAlive() == false ||
		//if level completed
		m_players[m_currPlayer]->GetGoalHit() == true)
	{
		m_currPlayer++;

		m_ticks = 0;

		if (m_currPlayer < m_players.size())
		{
			GameManager::GetGameMgr()->ChangePlayer(m_players[m_currPlayer].get());
			GameManager::GetGameMgr()->GetPlayer()->Reset();
		}
	}
	//if have moved right before timing out
	else if (m_players[m_currPlayer]->GetPosition().x != m_players[m_currPlayer]->GetPrevPosition().x && m_players[m_currPlayer]->GetPosition().x > 75.0f)
	{
		m_ticks = 0;
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
		for (int swp = 0; swp< m_players.size(); ++swp)
		{
			GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("Player: " + std::to_string(swp),false);
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
		for (int i = 0; i< m_numPlayers; ++i)
		{
			GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("Player " + std::to_string(i) + " Fitness: " + std::to_string(m_players[i]->Fitness()));

			m_players[i]->InsertNewBrain(pBrains[i]);
			m_players[i]->Reset();
		}

		m_currPlayer = 0;
		GameManager::GetGameMgr()->ChangePlayer(m_players[m_currPlayer].get());
		GameManager::GetGameMgr()->GetPlayer()->Reset();

		m_bestFitness = m_pop->BestEverFitness();

		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("Best Fitness: " + std::to_string(m_bestFitness));

		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("");
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("Current Generation: " + std::to_string(m_generations));
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(GameManager::GetGameMgr()->GetLogger()->GetTimeStamp());

		GameManager::GetGameMgr()->GetLogger()->AddDebugLog("");
		GameManager::GetGameMgr()->GetLogger()->AddDebugLog("Current Generation: " + std::to_string(m_generations));
	}

	return true;
}

std::vector<double> Controller::GetFitnessScores() const
{
	std::vector<double> scores;

	for (int i = 0; i< m_players.size(); ++i)
	{
		scores.push_back(m_players[i]->Fitness());
	}

	return scores;
}

void Controller::EndOfRunCalculation(AutomatedPlayer* ply)
{
	float percent = 0;
	float endX = ply->GetPosition().x;
	float startX = ply->GetInitialPosition().x;

	if (endX < startX)
	{
		percent = -((endX / startX) * 100);
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player moved left by {}%!", percent));
	}
	else if (endX == startX)
	{
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player did not move!"));
	}
	else if (ply->GetGoalHit())
	{
		percent = 100;
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog("Player completed the level");
	}
	else
	{
		percent = ((endX - startX) / RightMost) * 100;
		GameManager::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Player completed {}% of the level!", percent));
	}

	ply->UpdateFitness(percent);
}
