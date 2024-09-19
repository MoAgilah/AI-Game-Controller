#pragma once
//------------------------------------------------------------------------
//
//	Name: Cga.h
//
//  Author: Mat Buckland 2002
//  Edited: Mo Agilah 2024
//
//  Desc: The evolutionary algorithm class  used in the implementation of
//        Kenneth Owen Stanley's and Risto Miikkulainen's NEAT idea.
//
//------------------------------------------------------------------------
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <memory>

#include "CParams.h"
#include "CSpecies.h"
#include "Genotype.h"
#include "Phenotype.h"

//------------------------------------------------------------------------
//
//  this structure is used in the creation of a network depth lookup
//  table.
//------------------------------------------------------------------------
struct SplitDepth
{
	double val;

	int    depth;

	SplitDepth(double v, int d)
		:val(v), depth(d) {}
};


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
class Cga
{
public:
	Cga(int	size, int inputs, int outputs, int cx, int cy);
	~Cga() = default;

	std::vector<CNeuralNet*> Epoch(const std::vector<double>& FitnessScores);

	//iterates through the population and creates the phenotypes
	std::vector<CNeuralNet*> CreatePhenotypes();

	//keeps a record of the n best genomes from the last population.
	//(used to display the best genomes)
	void StoreBestGenomes();

	//returns a std::vector of the n best phenotypes from the previous generation
	std::vector<CNeuralNet*> GetBestPhenotypesFromLastGeneration();

	//----------------------------------------------------accessor methods
	int NumSpecies() const { return (int)m_vecSpecies.size(); }

	double BestEverFitness() const { return m_dBestEverFitness; }

private:

	//used in Crossover
	void AddNeuronID(int nodeID, std::vector<int>& vec);

	//this function resets some values ready for the next epoch, kills off
	//all the phenotypes and any poorly performing species.
	void ResetAndKill();

	//separates each individual into its respective species by calculating
	//a compatibility score with every other member of the population and
	//niching accordingly. The function then adjusts the fitness scores of
	//each individual by species age and by sharing and also determines
	//how many offspring each individual should spawn.
	void SpeciateAndCalculateSpawnLevels();

	//adjusts the fitness scores depending on the number sharing the
	//species and the age of the species.
	void AdjustSpeciesFitnesses();

	CGenome Crossover(CGenome& mum, CGenome& dad);

	CGenome TournamentSelection(const int NumComparisons);

	//searches the lookup table for the dSplitY value of each node in the
	//genome and returns the depth of the network based on this figure
	int	CalculateNetDepth(const CGenome& gen);

	//sorts the population into descending fitness, keeps a record of the
	//best n genomes and updates any fitness statistics accordingly
	void SortAndRecord();

	//a recursive function used to calculate a lookup table of split
	//depths
	std::vector<SplitDepth> Split(double low, double high, int depth);

	//current generation
	int	m_iGeneration;

	int	m_iNextGenomeID;

	int	m_iNextSpeciesID;

	int	m_iPopSize;

	//local copies of client area
	int	cxClient;
	int cyClient;

	//index into the genomes for the fittest genome
	int	m_iFittestGenome;

	//adjusted fitness scores
	double	m_dTotFitAdj;
	double	m_dAvFitAdj;

	double	m_dBestEverFitness;

	//to keep track of innovations
	std::unique_ptr<CInnovation> m_pInnovation;

	//current population
	std::vector<CGenome>	m_vecGenomes;

	//keep a record of the last generations best genomes. (used to render
	//the best performers to the display if the user presses the 'B' key)
	std::vector<CGenome>	m_vecBestGenomes;

	//all the species
	std::vector<CSpecies>  m_vecSpecies;

	//this holds the precalculated split depths. They are used
	//to calculate a neurons x/y position for rendering and also
	//for calculating the flush depth of the network when a
	//phenotype is working in 'snapshot' mode.
	std::vector<SplitDepth> vecSplits;
};