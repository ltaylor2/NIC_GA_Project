#include "Individual.h"

/*
 
 ****************************
 *        Individual        *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 The Individual class contains code for an abstract individual class used in conjunction
 with a genetic algorithm (GeneticAlgorithm.cpp) or a population based incremental learning
 algorithm (PBIL.h). Individual subclasses can be used to define problem-specific behaviors.
 Here, a MAXSAT individual is defined to help solve maximum satisfiability problems
 with the two aformentioned algorithms.
 
*/

/*
 A constructor for a Maximum-Satisifiablity (MAXSAT) assignment individual,
 where bit assignments are equally random.
 Parameters:
        size: Size of the bitstring array
*/
MAXSATIndividual::MAXSATIndividual(int size)
	: data(size, 0)
{
	for (unsigned int i = 0; i < data.size(); i++)
		data[i] = (rand() > RAND_MAX / 2 ? 1 : 0);
}

/* 
 A constructor for a MAXSAT assignment individual, where bit assignments are
 based on a probability vector for a PBIL algorithm
 Parameters:
        probVect: Reference to the probability vector that will determine bit assignment
*/
MAXSATIndividual::MAXSATIndividual(const std::vector<double>& probVect)
    : data(probVect.size() - 1, 0)
{
    for (unsigned int i = 0; i < probVect.size(); i++) {
        data[i] = ((double)rand() / RAND_MAX < probVect[i] ? 1 : 0);
    }
}

/*
 A constructor for a MAXSAT assignment individual, where bit assignments are a clone
 of a passed bitset
 Parameters:
        data_: A bit array to be cloned in this individual
*/
MAXSATIndividual::MAXSATIndividual(std::vector<bool> data_)
	: data(data_)
{}

/* 
 Mutates the MAXSAT individual for a genetic algorithm, flipping the bit
 value at an index with certain probability
 Parameters:
        mutationProb: Probability that the bit at an index will be flipped
*/
void MAXSATIndividual::mutate(double mutationProb)
{
    for (unsigned int i = 0; i < data.size(); i++) {
        if (static_cast<double>(rand()) / RAND_MAX < mutationProb) { 
            data[i] = !data[i];
        }
    }
}

/*
 Breeds the MAXSAT individual and a given mate, producing a pair of crossed-over offspring
 Parameters:
        method: Parsed info that determines how the indiviudals will crossover
        mate: Reference to an second individual that will be crossed with
 Returns the pair of pointers to the crossed offspring
*/
std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>
MAXSATIndividual::breed(CrossoverMethod method, const Individual& mate) const
{
    // Construct an individual with the given mate
    const MAXSATIndividual& satMate = dynamic_cast<const MAXSATIndividual&>(mate);
    
    // Initialize two vectors with for the indiviudal and its mate
    std::vector<bool> bred1(data);
    std::vector<bool> bred2(satMate.data);

    // For "One Point" crossover, pick an index in the data to "cut" the individuals,
    // and swap the info between the individuals across that point
    if (method == CrossoverMethod::onePoint) {
        int crossPoint = static_cast<double>(rand()) / RAND_MAX * data.size();
        for (int i = 0; i < crossPoint; i++) {
            bred1[i] = satMate.data[i];
            bred2[i] = data[i];
        }
        
    // For "Uniform" crossover, decide with equal random chance to swap
    // the data at an index between the two individuals
    } else if (method == CrossoverMethod::uniform) {
        for (unsigned int i = 0; i < data.size(); i++) {
            if (rand() < RAND_MAX / 2) {
                bred1[i] = data[i];
                bred2[i] = satMate.data[i];
            } else {
                bred1[i] = satMate.data[i];
                bred2[i] = data[i];
            }
        }
    }

    // Produce and return two MAXSAT individuals from the crossed over offspring
    std::shared_ptr<Individual> bred1Pt(new MAXSATIndividual(bred1));
    std::shared_ptr<Individual> bred2Pt(new MAXSATIndividual(bred2));
    return std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>(bred1Pt, bred2Pt);
}

/*
 Evalutes the fitness of MAXSAT individuals by referencing the Conjunctive Normal Form clauses. 
 Fitness here is a simple count of how many clauses are satisfied by the boolean assignment 
 represented by the data in the MAXSAT individual
 Returns the fitness (num of satisified clauses)
*/
double MAXSATIndividual::evaluateFitness() const
{
    // NOTE clauses is a global that contains the MAXSAT problem
    // Loop over clauses and count number of true clauses
    int numCorrectClauses = 0;
    for (unsigned int i = 0; i < clauses.size(); i++) {
		for (unsigned int j = 0; j < clauses[i].size(); j++) {
            // If non-negated variable in clause is set to true, clause is true
            // If negatated variable in clause is set to false, clause is true
			if ((clauses[i][j] > 0 && data[abs(clauses[i][j]) - 1] == 1) 
				|| (clauses[i][j] < 0 && data[abs(clauses[i][j]) - 1] == 0)) {
					numCorrectClauses++;
					break;
			}
		}
	}

    // Return as double
	return static_cast<double>(numCorrectClauses);
}

/*
 Prints out the data contained in a MAXSAT individual to view the assignment
*/
void MAXSATIndividual::print() const
{
    for (unsigned int i = 0; i < data.size(); i++)
        std::cout << data[i];
    std::cout << std::endl;
}

/*
 Allows for cloning of the individual without calling the specific constructor,
 maintaining generality.
 Returns a pointer to the new clone individual
*/
std::shared_ptr<Individual> MAXSATIndividual::clone() const
{
    return std::shared_ptr<Individual>(new MAXSATIndividual(*this));
}

/*
 Returns the value at the index of the data (here for PBIL)
*/
int MAXSATIndividual::at(int index) const
{
    return data[index];
}

/*
 Allows for general construction of an individual given a sample individual, for 
 construction from a general PBIL algorithm.
 Parameters:
        probVect: Reference to the probability vector for PBIL individual generation
 Returns the pointer to the newly generated individual
*/
std::shared_ptr<Individual> MAXSATIndividual::generateIndividualPBIL(const std::vector<double>& probVect)
{
    return std::shared_ptr<Individual>(new MAXSATIndividual(probVect));
}
