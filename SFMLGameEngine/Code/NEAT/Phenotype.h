#pragma once
//-----------------------------------------------------------------------
//
//  Name: Phenotype.h
//
//  Author: Mat Buckland 2002
//  Edited: Mo Agilah 2024
//
//	Desc: definitions required for the creation of a neural network. To be
//        used in the implementation of Kenneth Owen Stanley's NEAT idea.
//
//-----------------------------------------------------------------------

#include <vector>
#include <math.h>
#define NOMINMAX
#include <windows.h>
#include <algorithm>

#include "CParams.h"
#include "Genes.h"
#include "Utils.h"

struct SNeuron;

//------------------------------------------------------------------------
//
//  SLink structure
//------------------------------------------------------------------------
struct SLink
{
	SLink(double dW, SNeuron* pIn, SNeuron* pOut, bool bRec)
		: dWeight(dW),
		pIn(pIn),
		pOut(pOut),
		bRecurrent(bRec)
	{}

	//is this link a recurrent link?
	bool    bRecurrent;

	//the connection weight
	double  dWeight;

	//pointers to the neurons this link connects
	SNeuron* pIn;
	SNeuron* pOut;
};

//------------------------------------------------------------------------
//
//  SNeuron
//------------------------------------------------------------------------
struct SNeuron
{
	SNeuron(neuron_type type, int id, double y, double x, double ActResponse)
		: NeuronType(type),
		iNeuronID(id),
		dSumActivation(0),
		dOutput(0),
		iPosX(0),
		iPosY(0),
		dSplitY(y),
		dSplitX(x),
		dActivationResponse(ActResponse)
	{}

	//what type of neuron is this?
	neuron_type   NeuronType;

	//used in visualization of the phenotype
	int iPosX;
	int iPosY;

	//its identification number
	int  iNeuronID;

	//sum of weights x inputs
	double dSumActivation;

	//the output from this neuron
	double dOutput;

	//sets the curvature of the sigmoid function
	double dActivationResponse;

	double dSplitY;
	double dSplitX;

	//all the links coming into this neuron
	std::vector<SLink> vecLinksIn;

	//and out
	std::vector<SLink> vecLinksOut;
};

//------------------------------------------------------------------------
//
// CNeuralNet
//------------------------------------------------------------------------
class CNeuralNet
{
public:
	CNeuralNet(std::vector<SNeuron*> neurons, int depth);

	~CNeuralNet();

	//you have to select one of these types when updating the network
   //If snapshot is chosen the network depth is used to completely
   //flush the inputs through the network. active just updates the
   //network each timestep
	enum run_type { snapshot, active };

	//update network for this clock cycle
	std::vector<double> Update(const std::vector<double>& inputs, const run_type type);

	//draws a graphical representation of the network to a user speciefied window
	void  DrawNet(HDC& surface, int cxLeft, int cxRight, int cyTop, int cyBot);

private:

	//the depth of the network
	int               m_iDepth;

	std::vector<SNeuron*>  m_vecpNeurons;
};