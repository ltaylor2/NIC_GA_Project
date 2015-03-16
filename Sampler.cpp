#include "Sampler.h"

/*
 
 ****************************
 *         Sampler          *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 This file contains code for the Sampler class, used in conjunction with a 
 Genetic Algorithm. The sampler takes a population, and its associated set of
 fitness evaluations, picks individuals for the breeding pool for the next 
 generation based on the specified selection method. Currently, three selection
 methods are implemented: Ranking, Tournament, and Boltzmann.
 
*/

/*
 Constructor for the Sampler, which takes a population of individuals, their fitness evaluations, and
 selects (with a given selection method) an individual to pass into the breeding pool
 Parameters:
        method_: Parsed info indentified which of the implemented selection methods will be used
        population_: Reference to the vector of pointers to abstract individuals
        evaluations_: Reference to the vector of fitness evaluations of the population
*/
Sampler::Sampler(SelectionMethod method_,
                   const std::vector<std::shared_ptr<Individual>>& population_,
                   const std::vector<double>& evaluations_)
    : method(method_), population(population_), evaluations(evaluations_), sum(0)
{
    // In preparation for boltzman, sum the values of e^fitness for the denominator
    if (method == SelectionMethod::boltzmann) {
        for (unsigned int i = 0; i < evaluations.size(); i++)
            sum += exp(evaluations[i]);
    }
    
    // In preparation for rank selection, hash the individuals and their ranks
    else if (method == SelectionMethod::ranking) {
        // Key: fitness, value: vector of individuals (in case individuals have equal fitnesses)
        // store map in descending order of keys
        rankSum = 0; // sum the ranks of the individuals
        
        for (unsigned int i = 0; i < population.size(); i++) {
            double fitness = evaluations[i];
            
            // If fitness not equal to any other, add individual to map
            if (ranks[fitness].size() == 0) {
                std::vector<std::shared_ptr<Individual>> rankVector;
                rankVector.push_back(population[i]);
                ranks[fitness] = rankVector;
            }
            
            // Else add individual to existing vector in map
            else {
                ranks[fitness].push_back(population[i]);
            }
            rankSum += i;
        }
    }
}

/*
 Runs the selection method passed to the constructor on the population of individuals,
 see Sampler.h for selection method information
 Returns a pointer to a selected individual
*/
std::shared_ptr<Individual> Sampler::select() const
{

    if (method == SelectionMethod::boltzmann) {
        
        double random = static_cast<double>(rand()) / RAND_MAX;
        double missed = 0;
        
        // Not an off by one error, see NOTE below
        for (unsigned int i = 0; i < evaluations.size() - 1; i++) {
            double prob = exp(evaluations[i]) / sum;
            
            // Runs in linear time, rather than assessing the probability of
            // each one in two loops, add the chance to skip the previous ones
            // to the chance to get the select the current one
            if (random <= prob + missed)
                return population[i]->clone();
            missed += prob;
        }
        
        // NOTE Although all the probabilites should sum to 1, they may not
        //      since we are doing floating point math. If the random number is
        //      also particulary large, no selection would be made. In this case,
        //      we return the last Individual in the population.
        return population[population.size() - 1]->clone();
    }
    
    if (method == SelectionMethod::ranking) {
        double random = static_cast<double>(rand()) / RAND_MAX;
        
        int count = 0; // count of number of individuals checked so far
        double missed = 0;
        
        // Iterate through the map in descending order of keys
        for (std::map<double, std::vector<std::shared_ptr<Individual>>, std::greater<double>>::const_iterator it = ranks.begin(); it != ranks.end(); ++it) {
            // For all individuals that have same key (fitness)
            for (unsigned int i = 0; i < it->second.size(); i++) {
                double prob = static_cast<double>(population.size() - count)/rankSum;
                if (random <= prob + missed) {
                    return it->second[i];
                }
                
                missed += prob;
                count++;
            }
        }
        
        // see NOTE above -- return the last Individual in the population
        return population[population.size() - 1];
    }
    
    if (method == SelectionMethod::tournament) {
        // select two random Individuals from population
        int random1 = static_cast<int>(rand() % population.size());
        int random2 = static_cast<int>(rand() % population.size());
        
        double fitness1 = evaluations[random1];
        double fitness2 = evaluations[random2];
        
        if (fitness1 > fitness2) {
            return population[random1];
        }
        else if (fitness1 < fitness2) {
            return population[random2];
        }
        
        // fitnesses are equal, choose individual randomly
        if (static_cast<double>(rand()) / RAND_MAX < 0.5)
            return population[random1];
        
        return population[random2];
    }
    
    // Return an error if the selection method was wrong
    std::cout << "SelectionMethod choice failed." << std::endl;
    return std::shared_ptr<Individual>(NULL);
}
