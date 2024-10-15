#pragma once
//-----------------------------------------------------------------------
//
//  Name: CInnovation.h
//
//  Author: Mat Buckland 2002
//  Edited: Mo Agilah 2024
//
//	Desc: class to handle genome innovations used in the implementation
//        of Kenneth Owen Stanley's and Risto Miikkulainen's NEAT idea.
//
//-----------------------------------------------------------------------
#include <vector>
#include <algorithm>

#include "Genotype.h"
#include "Phenotype.h"
#include "Utils.h"

struct SLinkGene;

//---------------------Innovation related structs/classes----------------
//
//------------------------------------------------------------------------
enum innov_type
{
	new_neuron,
	new_link
};

//------------------------------------------------------------------------
//
//  structure defining an innovation
//------------------------------------------------------------------------
struct	SInnovation
{
	SInnovation(int in, int out, innov_type t, int inov_id)
		: NeuronIn(in),
		NeuronOut(out),
		InnovationType(t),
		InnovationID(inov_id),
		NeuronID(0),
		dSplitX(0),
		dSplitY(0),
		NeuronType(none)
	{}

	SInnovation(SNeuronGene neuron, int innov_id, int neuron_id)
		: InnovationType(innov_type::new_neuron),
		InnovationID(innov_id),
		NeuronID(neuron_id),
		dSplitX(neuron.dSplitX),
		dSplitY(neuron.dSplitY),
		NeuronType(neuron.NeuronType),
		NeuronIn(-1),
		NeuronOut(-1)
	{}

	SInnovation(int in, int out, innov_type  t, int inov_id, neuron_type type, double x, double y)
		: NeuronIn(in),
		NeuronOut(out),
		InnovationType(t),
		InnovationID(inov_id),
		NeuronID(0),
		NeuronType(type),
		dSplitX(x),
		dSplitY(y)
	{}

	//new neuron or new link?
	innov_type  InnovationType;

	int         InnovationID;

	int         NeuronIn;
	int         NeuronOut;

	int         NeuronID;

	neuron_type NeuronType;

	//if the innovation is a neuron we need to keep a record
	//of its position in the tree (for display purposes)
	double      dSplitY;
	double		dSplitX;
};

//------------------------------------------------------------------------
//
//  CInnovation class used to keep track of all innovations created during
//  the populations evolution
//------------------------------------------------------------------------
class CInnovation
{
public:
	CInnovation(std::vector<SLinkGene>   start_genes,
		std::vector<SNeuronGene> start_neurons);

	//checks to see if this innovation has already occurred. If it has it
	//returns the innovation ID. If not it returns a negative value.
	int   CheckInnovation(int in, int out, innov_type type);

	//creates a new innovation and returns its ID
	int   CreateNewInnovation(int in, int out, innov_type type);

	//as above but includes adding x/y position of new neuron
	int   CreateNewInnovation(int         from,
		int         to,
		innov_type  InnovType,
		neuron_type NeuronType,
		double      x,
		double      y);

	//creates a BasicNeuron from the given neuron ID
	SNeuronGene CreateNeuronFromID(int id);

	//------------------------------------------------accessor methods
	int   GetNeuronID(int inv)const { return m_vecInnovs[inv].NeuronID; }

	void  Flush() { m_vecInnovs.clear(); return; }

	int   NextNumber(int num = 0)
	{
		m_NextInnovationNum += num;

		return m_NextInnovationNum;
	}

private:

	int                 m_NextNeuronID;
	int                 m_NextInnovationNum;
	std::vector<SInnovation> m_vecInnovs;
};