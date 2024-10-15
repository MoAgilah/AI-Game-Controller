#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../NEAT/Cga.h"
#include "../Controller/ANNView.h"
#include "../Game/Timer.h"

class Player;
class AutomatedPlayer;

class AIController
{
public:
	AIController();
	~AIController() = default;

	bool Update(float deltaTime);

	Player* GetCurrentPlayer();

	const std::vector<double>& GetGridInputs();
	std::vector<double> GetFitnessScores() const;

	ANNView* GetAnnView() { return m_AnnView.get(); }
	int GetCurrentPlayerNum() const { return m_currPlayer; }
	int GetCurrentGeneration() const { return m_generations; }
	double BestFitness() const { return m_bestFitness; }

private:

	double ColourToInput(sf::Color col);
	void EndOfRunCalculation(AutomatedPlayer* ply);

	int m_currPlayer = 0;
	int m_generations = 0;
	double m_bestFitness = 0;

	Timer m_timer;
	std::unique_ptr<Cga> m_pop;
	std::vector<double> m_inputs;
	std::unique_ptr<ANNView> m_AnnView;
	std::vector<std::shared_ptr<AutomatedPlayer>> m_players;
};