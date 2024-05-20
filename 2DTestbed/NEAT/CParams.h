#pragma once
//------------------------------------------------------------------------
//
//	Name: CParams.h
//
//  Author: Mat Buckland 2002
//
//  Desc: class to hold all the parameters used in this project. The values
//        are loaded in from an ini file when an instance of the class is
//        created.
//
//
//------------------------------------------------------------------------
#define NOMINMAX
#include <Windows.h>
#include <fstream>
#include <iostream>

class CParams
{

public:
	//-------------------------------------------------------------------
	//  general parameters
	//-------------------------------------------------------------------

	static double dPi;
	static double dHalfPi;
	static double dTwoPi;

	static int    WindowWidth;
	static int    WindowHeight;

	static int    iFramesPerSecond;
	//-------------------------------------------------------------------
	//  used to define the players
	//-------------------------------------------------------------------
	static int    iNumPlayers;

	//amount of sensors
	static int    iNumSensors;
	//--------------------------------------controller parameters
	//number of time steps we allow for each generation to live
	static int    iNumTicks;
	//----------------------------------------------------------------------
	// used in phenotype.h/cpp
	//----------------------------------------------------------------------
	static int    iNumInputs;
	static int    iNumOutputs;
	//bias value
	static double dBias;
	//starting value for the sigmoid response
	static double dSigmoidResponse;
	//----------------------------------------------------------------------
	// used in genotype.h/cpp
	//----------------------------------------------------------------------

	//number of times we try to find 2 unlinked nodes when adding a link.
	//see CGenome::AddLink()
	static int    iNumAddLinkAttempts;

	//number of attempts made to choose a node that is not an input
	//node and that does not already have a recurrently looped connection
	//to itself. See CGenome::AddLink()
	static int    iNumTrysToFindLoopedLink;

	//the number of attempts made to find an old link to prevent chaining
	//in CGenome::AddNeuron
	static int    iNumTrysToFindOldLink;

	//the chance, each epoch, that a neuron or link will be added to the
	//genome
	static double dChanceAddLink;
	static double dChanceAddNode;
	static double dChanceAddRecurrentLink;

	//mutation probabilities for mutating the weights in CGenome::Mutate()
	static double dMutationRate;
	static double dMaxWeightPerturbation;
	static double dProbabilityWeightReplaced;

	//probabilities for mutating the activation response
	static double dActivationMutationRate;
	static double dMaxActivationPerturbation;

	//the smaller the number the more species will be created
	static double dCompatibilityThreshold;

	//----------------------------------------------------------------------
	// used in CSpecies.h/cpp
	//----------------------------------------------------------------------

	//during fitness adjustment this is how much the fitnesses of
	//young species are boosted (eg 1.2 is a 20% boost)
	static double dYoungFitnessBonus;

	//if the species are below this age their fitnesses are boosted
	static int    iYoungBonusAgeThreshhold;

	//number of population to survive each epoch. (0.2 = 20%)
	static double dSurvivalRate;

	//if the species is above this age their fitness gets penalized
	static int    iOldAgeThreshold;

	//by this much
	static double dOldAgePenalty;

	//----------------------------------------------------------------------
	// used in Cga.h/cpp
	//----------------------------------------------------------------------

	//how long we allow a species to exist without any improvement
	static int    iNumGensAllowedNoImprovement;

	//maximum number of neurons permitted in the network
	static int    iMaxPermittedNeurons;

	static double dCrossoverRate;

	//---------------------------------------------

	//
	CParams() {
		Initialize();
	}

	bool Initialize()
	{
		dPi = 3.14159265358979;
		dHalfPi = dPi / 2;
		dTwoPi = dPi * 2;

		iNumInputs = 240;
		iNumOutputs = 6;

		return true;

	}
};