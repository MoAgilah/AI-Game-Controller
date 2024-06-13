#pragma once

#include <SFML/Graphics.hpp>
#include "../NEAT/Cga.h"

class ANNView;
class Sensors;
class Player;
class Controller
{
public:
	Controller();
	ANNView* GetAnnView();
	std::vector<double> GetGridInputs();

	int GetCurrentPlayerNum();
	int GetCurrentGeneration();
	double BestFitness();
	~Controller();

	Player* GetCurrentPlayer();
	bool Update();
	std::vector<double>  GetFitnessScores()const;//body needs to be written
private:
	void EndOfRunCalculation(Player* ply);

	int currPlayer;
	int	iNumPlayers;
	ANNView* m_AnnView;

	double ColourToInput(sf::Color col);
	std::vector<double> m_inputs;
	//storage for the entire population of chromosomes
	Cga*                 m_pPop;
	//stores the average fitness per generation
	std::vector<double> m_vecAvFitness;
	//stores the best fitness per generation
	std::vector<double> m_vecBestFitness;
	//best fitness ever
	double m_dBestFitness;
	//cycles per generation
	int m_iTicks;
	//generation counter
	int m_iGenerations;
	//array of players
	std::vector<Player*> m_vecMarios;

	//points container
	std::vector<sf::Vector2f> points;
};