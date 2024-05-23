#include "../Controller/Controller.h"
#include "../Collisions/Collisions.h"
#include "../GameObjects/Player.h"
#include "../Controller/ANNView.h"
#include "../Game/Game.h"

Controller::Controller()
{
	m_AnnView = new ANNView();

	iNumPlayers = CParams::iNumPlayers;
	currPlayer = 0;
	m_dBestFitness = 0;

	if (Automated)
	{
		//let's create the players
		for (int i = 0; i< iNumPlayers; ++i)
		{
			m_vecMarios.push_back(new Player("mario.png", 8, 2, false, 0, .5f));
		}

		m_pPop = new Cga(CParams::iNumPlayers,
			CParams::iNumInputs,
			CParams::iNumOutputs,
			10, 10);

		//create the phenotypes
		std::vector<CNeuralNet*> pBrains = m_pPop->CreatePhenotypes();

		//assign the phenotypes
		for (int i = 0; i< iNumPlayers; i++)
		{
			m_vecMarios[i]->InsertNewBrain(pBrains[i]);
		}
	}

	m_iTicks = 0;
	m_iGenerations = 0;

	m_inputs.resize(240);
}

ANNView * Controller::GetAnnView()
{
	return m_AnnView;
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
	std::vector<Tile*> tiles = m_AnnView->GetVecView();

	for (int i = 0; i < tiles.size(); i++)
	{
		m_inputs[i] = ColourToInput(tiles[i]->GetRect().getFillColor());
	}

	return m_inputs;
}

int Controller::GetCurrentPlayerNum()
{
	return currPlayer;
}

int Controller::GetCurrentGeneration()
{
	return m_iGenerations;
}

double Controller::BestFitness()
{
	return m_dBestFitness;
}

Controller::~Controller()
{
	if (m_AnnView)
	{
		delete m_AnnView;
		m_AnnView = nullptr;
	}
}

Player * Controller::GetCurrentPlayer()
{
	return m_vecMarios[currPlayer];
}

bool Controller::Update()
{
	//if havent moved in 2000 ticks
	if ((m_vecMarios[currPlayer]->GetPosition().x == m_vecMarios[currPlayer]->GetPrevPostion().x && m_iTicks++ > CParams::iNumTicks) ||
		//if been killed
		m_vecMarios[currPlayer]->GetIsAlive() == false ||
		//if level completed
		m_vecMarios[currPlayer]->GetGoalHit() == true)
	{
		currPlayer++;

		m_iTicks = 0;

		if (currPlayer < m_vecMarios.size())
		{
			Game::GetGameMgr()->ChangePlayer(m_vecMarios[currPlayer]);
			Game::GetGameMgr()->GetPlayer()->ReSpawn();
		}
	}
	//if have moved right before timing out
	else if (m_vecMarios[currPlayer]->GetPosition().x != m_vecMarios[currPlayer]->GetPrevPostion().x && m_vecMarios[currPlayer]->GetPosition().x > 75.0f)
	{
		m_iTicks = 0;
	}

	if (currPlayer < m_vecMarios.size())
	{
		if (!m_vecMarios[currPlayer]->UpdateANN())
		{
			//error in processing the neural net
			std::cout << "Wrong amount of NN inputs!" << std::endl;

			return false;
		}
	}

	//We have completed another generation so now we need to run the GA
	if (currPlayer == m_vecMarios.size())
	{
		//first add up each players fitness scores. (remember for this task
		//there are many different sorts of penalties the players may incur
		//and each one has a coefficient)
		for (int swp = 0; swp< m_vecMarios.size(); ++swp)
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog("Player: " + std::to_string(swp),false);
			m_vecMarios[swp]->EndOfRunCalculations();
		}

		//increment the generation counter
		++m_iGenerations;

		//reset cycles
		m_iTicks = 0;

		//perform an epoch and grab the new brains
		std::vector<CNeuralNet*> pBrains = m_pPop->Epoch(GetFitnessScores());

		//insert the new  brains back into the players and reset their
		//state
		for (int i = 0; i< iNumPlayers; ++i)
		{
			Game::GetGameMgr()->GetLogger()->AddExperimentLog("Player " + std::to_string(i) + " Fitness: " + std::to_string(m_vecMarios[i]->Fitness()));

			m_vecMarios[i]->InsertNewBrain(pBrains[i]);
			m_vecMarios[i]->Reset();
		}

		currPlayer = 0;
		Game::GetGameMgr()->ChangePlayer(m_vecMarios[currPlayer]);
		Game::GetGameMgr()->GetPlayer()->ReSpawn();

		m_dBestFitness = m_pPop->BestEverFitness();

		Game::GetGameMgr()->GetLogger()->AddExperimentLog("Best Fitness: " + std::to_string(m_dBestFitness));

		Game::GetGameMgr()->GetLogger()->AddExperimentLog("");
		Game::GetGameMgr()->GetLogger()->AddExperimentLog("Current Generation: " + std::to_string(m_iGenerations));
		Game::GetGameMgr()->GetLogger()->AddExperimentLog(Game::GetGameMgr()->GetLogger()->GetTimeStamp());

		Game::GetGameMgr()->GetLogger()->AddDebugLog("");
		Game::GetGameMgr()->GetLogger()->AddDebugLog("Current Generation: " + std::to_string(m_iGenerations));
	}

	return true;
}

std::vector<double> Controller::GetFitnessScores() const
{
	std::vector<double> scores;

	for (int i = 0; i< m_vecMarios.size(); ++i)
	{
		scores.push_back(m_vecMarios[i]->Fitness());
	}

	return scores;
}
