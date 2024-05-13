#pragma once

#include <SFML/Graphics.hpp>
#include "../Utils.h"

#include "../NEAT/Cga.h"

class ANNView;
class Sensors;
class Player;
class Controller
{
public:
	Controller();
	//void CheckSensors();
	ANNView* GetAnnView();
	std::vector<double> GetGridInputs();
	std::vector<double> GetSensorInputs();
	std::vector<int> GetOutputs();

	int GetCurrentPlayerNum();
	int GetCurrentGeneration();
	double BestFitness();
	~Controller();
	
	Player* GetCurrentPlayer();
	bool Update();
	std::vector<double>  GetFitnessScores()const;//body needs to be written
private:
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