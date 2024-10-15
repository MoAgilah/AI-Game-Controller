#include "CInnovation.h"

//---------------------------------- ctor --------------------------------
//
//  given a series of start genes and start neurons this ctor adds
//  all the appropriate innovations.
//------------------------------------------------------------------------
CInnovation::CInnovation(std::vector<SLinkGene>   start_genes,
	std::vector<SNeuronGene> start_neurons)
{
	m_NextNeuronID = 0;
	m_NextInnovationNum = 0;

	//add the neurons
	for (auto& neuron : start_neurons)
	{
		m_vecInnovs.push_back(SInnovation(neuron,
			m_NextInnovationNum++,
			m_NextNeuronID++));
	}

	//add the links
	for (auto& gen : start_genes)
	{
		SInnovation NewInnov(gen.FromNeuron,
			gen.ToNeuron,
			new_link,
			m_NextInnovationNum);

		m_vecInnovs.push_back(NewInnov);

		++m_NextInnovationNum;

	}
}

//---------------------------CheckInnovation------------------------------
//
//	checks to see if this innovation has already occurred. If it has it
//	returns the innovation ID. If not it returns a negative value.
//------------------------------------------------------------------------
int CInnovation::CheckInnovation(int in, int out, innov_type type)
{
	//iterate through the innovations looking for a match on all
	//three parameters
	for (auto& inv : m_vecInnovs)
	{
		if ((inv.NeuronIn == in) &&
			(inv.NeuronOut == out) &&
			(inv.InnovationType == type))
		{
			//found a match so assign this innovation number to id
			return inv.InnovationID;
		}
	}

	//if no match return a negative value
	return -1;
}

//--------------------------CreateNewInnovation---------------------------
//
//	creates a new innovation and returns its ID
//------------------------------------------------------------------------
int CInnovation::CreateNewInnovation(int in, int out, innov_type type)
{
	SInnovation new_innov(in, out, type, m_NextInnovationNum);

	if (type == new_neuron)
	{
		new_innov.NeuronID = m_NextNeuronID;

		++m_NextNeuronID;
	}

	m_vecInnovs.push_back(new_innov);

	++m_NextInnovationNum;

	return (m_NextNeuronID - 1);
}

//------------------------------------------------------------------------
//
//  as above but includes adding x/y position of new neuron
//------------------------------------------------------------------------
int CInnovation::CreateNewInnovation(int from, int to, innov_type InnovType,
	neuron_type NeuronType, double  x, double y)
{
	SInnovation new_innov(from, to, InnovType, m_NextInnovationNum, NeuronType, x, y);

	if (InnovType == new_neuron)
	{
		new_innov.NeuronID = m_NextNeuronID;

		++m_NextNeuronID;
	}

	m_vecInnovs.push_back(new_innov);

	++m_NextInnovationNum;

	return (m_NextNeuronID - 1);

}


//------------------------------- CreateNeuronFromID -----------------------
//
//  given a neuron ID this function returns a clone of that neuron
//------------------------------------------------------------------------
SNeuronGene CInnovation::CreateNeuronFromID(int NeuronID)
{
	SNeuronGene temp(hidden, 0, 0, 0);

	for (auto inv : m_vecInnovs)
	{
		if (inv.NeuronID == NeuronID)
		{
			temp.NeuronType = inv.NeuronType;
			temp.iID = inv.NeuronID;
			temp.dSplitY = inv.dSplitY;
			temp.dSplitX = inv.dSplitX;

			return temp;
		}
	}

	return temp;
}