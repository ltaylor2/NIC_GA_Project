#pragma once

#include "Individual.h"

#include <vector>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <map>

/*
 
 ****************************
 *         Sampler          *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 See Sampler.cpp for more documentation.
 
*/

enum class SelectionMethod { ranking, tournament, boltzmann };

/* Class for sampling from a population given a selection method
 (1) SelectionMethod::ranking - Rank selection ranks the individuals (here hashed by rank in the constructor),
                                 from lowest (1) to highest (N), then selects with probability [rank[i] / sum(ranks[i])
 
 (2) SelectionMethod::tournament - randomly selects M individuals, and passes
                                    the best k(Fitness) < M individuals to the breeding pool. Here,
                                    each tournament selects the best k = 1 of M = 2 individuals, returning
                                    both to the population for possible future selection
 
 (3) SelectionMethod::boltzmann - selects an individual with probability[e^fitness[i] / sum(e^fitnesses[i])]
                                    (here the sum of the ranks occurs in the constructor)
 
 Class and method documentation can be found in Sampler.cpp
*/
class Sampler {
public:
    Sampler(SelectionMethod method_, const std::vector<std::shared_ptr<Individual>>& population_, const std::vector<double>& evaluations_);
    std::shared_ptr<Individual> select() const;

private:
    SelectionMethod method;
    
    // References to the population and its fitness evaluations
    const std::vector<std::shared_ptr<Individual>>& population;
    const std::vector<double>& evaluations;
    
    // A hashtable to store individuals by fitness rank for ranking selection, and its iterator
    std::map<double, std::vector<std::shared_ptr<Individual>>, std::greater<double>> ranks;
    int rankSum;
    
    // Value of the exponential sum of fitnesses for boltzmann selection
    double sum;
};
