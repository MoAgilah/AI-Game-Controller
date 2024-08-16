#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Controller/ANNView.h"
#include "../NEAT/Cga.h"

class Player;
class AutomatedPlayer;
class Controller
{
public:
	Controller();
	~Controller() = default;

	ANNView* GetAnnView() { return m_AnnView.get(); }
	std::vector<double> GetGridInputs();

	int GetCurrentPlayerNum() const { return m_currPlayer; }
	int GetCurrentGeneration() const { return m_generations; }
	double BestFitness() const { return m_bestFitness; }

	Player* GetCurrentPlayer();
	bool Update();
	std::vector<double>  GetFitnessScores() const;

private:

	void EndOfRunCalculation(AutomatedPlayer* ply);
	double ColourToInput(sf::Color col);

	int m_ticks;
	int m_currPlayer;
	int	m_numPlayers;
	int m_generations;
	double m_bestFitness;

	std::unique_ptr<Cga> m_pop;
	std::vector<double> m_inputs;
	std::unique_ptr<ANNView> m_AnnView;
	std::vector<std::shared_ptr<AutomatedPlayer>> m_players;
};