#include "Cga.h"


//-------------------------------------------------------------------------
//	this constructor creates a base genome from supplied values and creates
//	a population of 'size' similar (same topology, varying weights) genomes
//-------------------------------------------------------------------------
Cga::Cga(int	size, int inputs, int outputs, int cx, int cy)
	: m_iPopSize(size),
	m_iGeneration(0),
	m_iNextGenomeID(0),
	m_iNextSpeciesID(0),
	m_iFittestGenome(0),
	m_dBestEverFitness(0),
	m_dTotFitAdj(0),
	m_dAvFitAdj(0),
	cxClient(cx),
	cyClient(cy)
{
	//create the population of genomes
	for (int i = 0; i < m_iPopSize; ++i)
	{
		m_vecGenomes.push_back(CGenome(m_iNextGenomeID++, inputs, outputs));
	}

	//create the innovation list. First create a minimal genome
	CGenome genome(1, inputs, outputs);

	//create the innovations
	m_pInnovation = std::make_unique<CInnovation>(genome.Genes(), genome.Neurons());

	//create the network depth lookup table
	vecSplits = Split(0, 1, 0);
}

//-------------------------------CreatePhenotypes-------------------------
//
//	cycles through all the members of the population and creates their
//  phenotypes. Returns a std::std::vector containing pointers to the new phenotypes
//-------------------------------------------------------------------------
std::vector<CNeuralNet*> Cga::CreatePhenotypes()
{
	std::vector<CNeuralNet*> networks;

	for (int i = 0; i < m_iPopSize; i++)
	{
		//calculate max network depth
		int depth = CalculateNetDepth(m_vecGenomes[i]);

		//create new phenotype
		CNeuralNet* net = m_vecGenomes[i].CreatePhenotype(depth);

		networks.push_back(net);
	}

	return networks;
}

//-------------------------- CalculateNetDepth ---------------------------
//
//  searches the lookup table for the dSplitY value of each node in the
//  genome and returns the depth of the network based on this figure
//------------------------------------------------------------------------
int Cga::CalculateNetDepth(const CGenome& gen)
{
	int MaxSoFar = 0;

	for (int nd = 0; nd < gen.NumNeurons(); ++nd)
	{
		for (int i = 0; i < vecSplits.size(); ++i)
		{

			if ((gen.SplitY(nd) == vecSplits[i].val) &&
				(vecSplits[i].depth > MaxSoFar))
			{
				MaxSoFar = vecSplits[i].depth;
			}
		}
	}
	return MaxSoFar + 2;
}


//-----------------------------------AddNeuronID----------------------------
//
//	just checks to see if a node ID has already been added to a std::vector of
//  nodes. If not 	then the new ID  gets added. Used in Crossover.
//------------------------------------------------------------------------
void Cga::AddNeuronID(const int nodeID, std::vector<int>& vec)
{
	for (auto& v : vec)
	{
		if (v == nodeID)
		{
			//already added
			return;
		}
	}

	vec.push_back(nodeID);
}

//------------------------------------- Epoch ----------------------------
//
//  This function performs one epoch of the genetic algorithm and returns
//  a std::vector of pointers to the new phenotypes
//------------------------------------------------------------------------
std::vector<CNeuralNet*> Cga::Epoch(const std::vector<double>& FitnessScores)
{
	//first check to make sure we have the correct amount of fitness scores
	if (FitnessScores.size() != m_vecGenomes.size())
	{
		std::cout << "Cga::Epoch(scores/ genomes mismatch)!" << std::endl;
	}

	//reset appropriate values and kill off the existing phenotypes and
	//any poorly performing species
	ResetAndKill();

	//update the genomes with the fitnesses scored in the last run
	for (int gen = 0; gen < m_vecGenomes.size(); ++gen)
	{
		m_vecGenomes[gen].SetFitness(FitnessScores[gen]);
	}

	//sort genomes and keep a record of the best performers
	SortAndRecord();

	//separate the population into species of similar topology, adjust
	//fitnesses and calculate spawn levels
	SpeciateAndCalculateSpawnLevels();

	//this will hold the new population of genomes
	std::vector<CGenome> NewPop;

	//request the offspring from each species. The number of children to
	//spawn is a double which we need to convert to an int.
	int NumSpawnedSoFar = 0;

	CGenome baby;

	//now to iterate through each species selecting offspring to be mated and
	//mutated
	for (auto& spc : m_vecSpecies)
	{
		//because of the number to spawn from each species is a double
		//rounded up or down to an integer it is possible to get an overflow
		//of genomes spawned. This statement just makes sure that doesn't
		//happen
		if (NumSpawnedSoFar < CParams::iNumPlayers)
		{
			//this is the amount of offspring this species is required to
			// spawn. Rounded simply rounds the double up or down.
			int NumToSpawn = Rounded(spc.NumToSpawn());

			bool bChosenBestYet = false;

			while (NumToSpawn--)
			{
				//first grab the best performing genome from this species and transfer
				//to the new population without mutation. This provides per species
				//elitism
				if (!bChosenBestYet)
				{
					baby = spc.Leader();

					bChosenBestYet = true;
				}
				else
				{
					//if the number of individuals in this species is only one
					//then we can only perform mutation
					if (spc.NumMembers() == 1)
					{
						//spawn a child
						baby = spc.Spawn();
					}
					//if greater than one we can use the crossover operator
					else
					{
						//spawn1
						CGenome g1 = spc.Spawn();

						if (RandFloat() < CParams::dCrossoverRate)
						{

							//spawn2, make sure it's not the same as g1
							CGenome g2 = spc.Spawn();

							//number of attempts at finding a different genome
							int NumAttempts = 5;

							while ((g1.ID() == g2.ID()) && (NumAttempts--))
							{
								g2 = spc.Spawn();
							}

							if (g1.ID() != g2.ID())
							{
								baby = Crossover(g1, g2);
							}
						}
						else
						{
							baby = g1;
						}
					}


					++m_iNextGenomeID;

					baby.SetID(m_iNextGenomeID);

					//now we have a spawned child lets mutate it! First there is the
					//chance a neuron may be added
					if (baby.NumNeurons() < CParams::iMaxPermittedNeurons)
					{
						baby.AddNeuron(CParams::dChanceAddNode,
							*m_pInnovation,
							CParams::iNumTrysToFindOldLink);
					}

					//now there's the chance a link may be added
					baby.AddLink(CParams::dChanceAddLink,
						CParams::dChanceAddRecurrentLink,
						*m_pInnovation,
						CParams::iNumTrysToFindLoopedLink,
						CParams::iNumAddLinkAttempts);

					//mutate the weights
					baby.MutateWeights(CParams::dMutationRate,
						CParams::dProbabilityWeightReplaced,
						CParams::dMaxWeightPerturbation);

					baby.MutateActivationResponse(CParams::dActivationMutationRate,
						CParams::dMaxActivationPerturbation);
				}

				//sort the babies genes by their innovation numbers
				baby.SortGenes();

				//add to new pop
				NewPop.push_back(baby);

				++NumSpawnedSoFar;

				if (NumSpawnedSoFar == CParams::iNumPlayers)
				{
					NumToSpawn = 0;
				}
			}//end while
		}//end if
	}//next species


	//if there is an underflow due to the rounding error and the amount
	//of offspring falls short of the population size additional children
	//need to be created and added to the new population. This is achieved
	//simply, by using tournament selection over the entire population.
	if (NumSpawnedSoFar < CParams::iNumPlayers)
	{
		//calculate amount of additional children required
		int Rqd = CParams::iNumPlayers - NumSpawnedSoFar;

		//grab them
		while (Rqd--)
		{
			NewPop.push_back(TournamentSelection(m_iPopSize / 5));
		}
	}

	//replace the current population with the new one
	m_vecGenomes = NewPop;

	//create the new phenotypes
	std::vector<CNeuralNet*> new_phenotypes;

	for(auto& gen : m_vecGenomes)
	{
		//calculate max network depth
		int depth = CalculateNetDepth(gen);

		CNeuralNet* phenotype = gen.CreatePhenotype(depth);

		new_phenotypes.push_back(phenotype);
	}

	//increase generation counter
	++m_iGeneration;

	return new_phenotypes;
}

//--------------------------- SortAndRecord-------------------------------
//
//  sorts the population into descending fitness, keeps a record of the
//  best n genomes and updates any fitness statistics accordingly
//------------------------------------------------------------------------
void Cga::SortAndRecord()
{
	//sort the genomes according to their unadjusted (no fitness sharing)
	//fitnesses
	sort(m_vecGenomes.begin(), m_vecGenomes.end());

	//is the best genome this generation the best ever?
	if (m_vecGenomes[0].Fitness() > m_dBestEverFitness)
	{
		m_dBestEverFitness = m_vecGenomes[0].Fitness();
	}

	//keep a record of the n best genomes
	StoreBestGenomes();
}

//----------------------------- StoreBestGenomes -------------------------
//
//  used to keep a record of the previous populations best genomes so that
//  they can be displayed if required.
//------------------------------------------------------------------------
void Cga::StoreBestGenomes()
{
	//clear old record
	m_vecBestGenomes.clear();

	//numbestgenomes not sure if it will be kept
	for (int gen = 0; gen < 4; ++gen)
	{
		m_vecBestGenomes.push_back(m_vecGenomes[gen]);
	}
}

//----------------- GetBestPhenotypesFromLastGeneration ------------------
//
//  returns a std::std::vector of the n best phenotypes from the previous
//  generation
//------------------------------------------------------------------------
std::vector<CNeuralNet*> Cga::GetBestPhenotypesFromLastGeneration()
{
	std::vector<CNeuralNet*> brains;

	for (auto& gen : m_vecBestGenomes)
	{
		//calculate max network depth
		int depth = CalculateNetDepth(gen);

		brains.push_back(gen.CreatePhenotype(depth));
	}

	return brains;
}

//--------------------------- AdjustSpecies ------------------------------
//
//  this functions simply iterates through each species and calls
//  AdjustFitness for each species
//------------------------------------------------------------------------
void Cga::AdjustSpeciesFitnesses()
{
	for (auto& spc : m_vecSpecies)
	{
		spc.AdjustFitnesses();
	}
}

//------------------ SpeciateAndCalculateSpawnLevels ---------------------
//
//  separates each individual into its respective species by calculating
//  a compatibility score with every other member of the population and
//  niching accordingly. The function then adjusts the fitness scores of
//  each individual by species age and by sharing and also determines
//  how many offspring each individual should spawn.
//------------------------------------------------------------------------
void Cga::SpeciateAndCalculateSpawnLevels()
{
	bool bAdded = false;

	//iterate through each genome and speciate
	for (int gen = 0; gen < m_vecGenomes.size(); ++gen)
	{
		//calculate its compatibility score with each species leader. If
		//compatible add to species. If not, create a new species
		for (int spc = 0; spc < m_vecSpecies.size(); ++spc)
		{
			double compatibility = m_vecGenomes[gen].GetCompatibilityScore(m_vecSpecies[spc].Leader());

			//if this individual is similar to this species add to species
			if (compatibility <= CParams::dCompatibilityThreshold)
			{
				m_vecSpecies[spc].AddMember(m_vecGenomes[gen]);

				//let the genome know which species it's in
				m_vecGenomes[gen].SetSpecies(m_vecSpecies[spc].ID());

				bAdded = true;

				break;
			}
		}

		if (!bAdded)
		{
			//we have not found a compatible species so let's create a new one
			m_vecSpecies.push_back(CSpecies(m_vecGenomes[gen], m_iNextSpeciesID++));
		}

		bAdded = false;
	}

	//now all the genomes have been assigned a species the fitness scores
	//need to be adjusted to take into account sharing and species age.
	AdjustSpeciesFitnesses();

	//calculate new adjusted total & average fitness for the population
	for (auto& gen : m_vecGenomes)
	{
		m_dTotFitAdj += gen.GetAdjFitness();
	}

	m_dAvFitAdj = m_dTotFitAdj / m_vecGenomes.size();

	//calculate how many offspring each member of the population
	//should spawn
	for (auto& gen : m_vecGenomes)
	{
		double ToSpawn = gen.GetAdjFitness() / m_dAvFitAdj;

		gen.SetAmountToSpawn(ToSpawn);
	}

	//iterate through all the species and calculate how many offspring
	//each species should spawn
	for (auto& spc : m_vecSpecies)
	{
		spc.CalculateSpawnAmount();
	}
}


//--------------------------- TournamentSelection ------------------------
//
//------------------------------------------------------------------------
CGenome Cga::TournamentSelection(const int NumComparisons)
{
	double BestFitnessSoFar = 0;

	int ChosenOne = 0;

	//Select NumComparisons members from the population at random testing
	//against the best found so far
	for (int i = 0; i < NumComparisons; ++i)
	{
		int ThisTry = RandInt(0, (int)m_vecGenomes.size() - 1);

		if (m_vecGenomes[ThisTry].Fitness() > BestFitnessSoFar)
		{
			ChosenOne = ThisTry;

			BestFitnessSoFar = m_vecGenomes[ThisTry].Fitness();
		}
	}

	//return the champion
	return m_vecGenomes[ChosenOne];
}

//-----------------------------------Crossover----------------------------
//
//------------------------------------------------------------------------
CGenome Cga::Crossover(CGenome& mum, CGenome& dad)
{
	//helps make the code clearer
	enum parent_type { MUM, DAD, };

	//first, calculate the genome we will using the disjoint/excess
	//genes from. This is the fittest genome.
	parent_type best;

	//if they are of equal fitness use the shorter (because we want to keep
	//the networks as small as possible)
	if (mum.Fitness() == dad.Fitness())
	{
		//if they are of equal fitness and length just choose one at
		//random
		if (mum.NumGenes() == dad.NumGenes())
		{
			best = (parent_type)RandInt(0, 1);
		}
		else
		{
			if (mum.NumGenes() < dad.NumGenes())
			{
				best = MUM;
			}
			else
			{
				best = DAD;
			}
		}
	}
	else
	{
		if (mum.Fitness() > dad.Fitness())
		{
			best = MUM;
		}
		else
		{
			best = DAD;
		}
	}

	//these std::vectors will hold the offspring's nodes and genes
	std::vector<SNeuronGene>  BabyNeurons;
	std::vector<SLinkGene>    BabyGenes;

	//temporary std::vector to store all added node IDs
	std::vector<int> vecNeurons;

	//create iterators so we can step through each parents genes and set
	//them to the first gene of each parent
	std::vector<SLinkGene>::iterator curMum = mum.StartOfGenes();
	std::vector<SLinkGene>::iterator curDad = dad.StartOfGenes();

	//this will hold a copy of the gene we wish to add at each step
	SLinkGene SelectedGene;

	//step through each parents genes until we reach the end of both
	while (!((curMum == mum.EndOfGenes()) && (curDad == dad.EndOfGenes())))
	{
		//the end of mum's genes have been reached
		if ((curMum == mum.EndOfGenes()) && (curDad != dad.EndOfGenes()))
		{
			//if dad is fittest
			if (best == DAD)
			{
				//add dads genes
				SelectedGene = *curDad;
			}

			//move onto dad's next gene
			++curDad;
		}
		//the end of dads's genes have been reached
		else if ((curDad == dad.EndOfGenes()) && (curMum != mum.EndOfGenes()))
		{
			//if mum is fittest
			if (best == MUM)
			{
				//add mums genes
				SelectedGene = *curMum;
			}

			//move onto mum's next gene
			++curMum;
		}
		//if mums innovation number is less than dads
		else if (curMum->InnovationID < curDad->InnovationID)
		{
			//if mum is fittest add gene
			if (best == MUM)
			{
				SelectedGene = *curMum;
			}

			//move onto mum's next gene
			++curMum;
		}
		//if dads innovation number is less than mums
		else if (curDad->InnovationID < curMum->InnovationID)
		{
			//if dad is fittest add gene
			if (best == DAD)
			{
				SelectedGene = *curDad;
			}

			//move onto dad's next gene
			++curDad;
		}
		//if innovation numbers are the same
		else if (curDad->InnovationID == curMum->InnovationID)
		{
			//grab a gene from either parent
			if (RandFloat() < 0.5f)
			{
				SelectedGene = *curMum;
			}
			else
			{
				SelectedGene = *curDad;
			}

			//move onto next gene of each parent
			++curMum;
			++curDad;
		}

		//add the selected gene if not already added
		if (BabyGenes.size() == 0)
		{
			BabyGenes.push_back(SelectedGene);
		}
		else
		{
			if (BabyGenes[BabyGenes.size() - 1].InnovationID !=
				SelectedGene.InnovationID)
			{
				BabyGenes.push_back(SelectedGene);
			}
		}

		//Check if we already have the nodes referred to in SelectedGene.
		//If not, they need to be added.
		AddNeuronID(SelectedGene.FromNeuron, vecNeurons);
		AddNeuronID(SelectedGene.ToNeuron, vecNeurons);

	}//end while

	//now create the required nodes. First sort them into order
	sort(vecNeurons.begin(), vecNeurons.end());

	for (auto& neuron : vecNeurons)
	{
		BabyNeurons.push_back(m_pInnovation->CreateNeuronFromID(neuron));
	}

	//finally, create the genome
	CGenome babyGenome(m_iNextGenomeID++,
		BabyNeurons,
		BabyGenes,
		mum.NumInputs(),
		mum.NumOutputs());

	return babyGenome;
}


//--------------------------- ResetAndKill -------------------------------
//
//  This function resets some values ready for the next epoch, kills off
//  all the phenotypes and any poorly performing species.
//------------------------------------------------------------------------
void Cga::ResetAndKill()
{
	m_dTotFitAdj = 0;
	m_dAvFitAdj = 0;

	//purge the species
	std::vector<CSpecies>::iterator curSp = m_vecSpecies.begin();

	while (curSp != m_vecSpecies.end())
	{
		curSp->Purge();

		//kill off species if not improving and if not the species which contains
		//the best genome found so far
		if ((curSp->GensNoImprovement() > CParams::iNumGensAllowedNoImprovement) &&
			(curSp->BestFitness() < m_dBestEverFitness))
		{
			curSp = m_vecSpecies.erase(curSp);
			--curSp;
		}

		++curSp;
	}

	//we can also delete the phenotypes
	for (auto& gen : m_vecGenomes)
	{
		gen.DeletePhenotype();
	}
}

//------------------------------- Split ----------------------------------
//
//  this function is used to create a lookup table that is used to
//  calculate the depth of the network.
//------------------------------------------------------------------------
std::vector<SplitDepth> Cga::Split(double low, double high, int depth)
{
	static std::vector<SplitDepth> vecSplits;

	double span = high - low;

	vecSplits.push_back(SplitDepth(low + span / 2, depth + 1));

	if (depth > 6)
	{
		return vecSplits;
	}
	else
	{
		Split(low, low + span / 2, depth + 1);
		Split(low + span / 2, high, depth + 1);

		return vecSplits;
	}
}