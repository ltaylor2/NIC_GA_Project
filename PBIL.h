#include "Individual.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <utility>

/*
 
 ****************************
 *          PBIL            *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 See PBIL.cpp for class and method documentation.
 
*/

// A generalized population based incremental learning algorithm that works with abstract Individuals
class PBIL {
public:
	PBIL(
         int probVectSize,
         std::shared_ptr<Individual> sampleIndividual,
		 int numIndividuals,
		 double posLR,
		 double negLR,
		 double mutationProb,
		 double mutationShift,
		 int numIter,
         double maxSolution_);

private:
	void run(std::shared_ptr<Individual> sampleIndividual,
             int numIndividuals,
			 double posLR,
			 double negLR,
			 double mutationProb,
			 double mutationShift,
			 int numIter);

	void printSuccess() const;
    void printEnd() const;
    
	void findBestAndWorst();
	void updateProbVect(double posLR, double NegLR);
	void mutateProbVect(double mutationProb, double mutationShift);

	int currIter;
    
    double maxSolution;
    bool success;

    // The probability vector itself
	std::vector<double> probVect;
    
    // The evaluations and the actual population of individuals
	std::vector<double> evaluations;
    std::vector<std::shared_ptr<Individual>> population;
    
    // Best and worst individuals of a given generation
	std::shared_ptr<Individual> bestSolution;
	std::shared_ptr<Individual> worstSolution;
    int bestEvalIndex;
	int worstEvalIndex;

    // Best ever individual, and when it occured
    std::shared_ptr<Individual> topSolution;
    int topIter;
    double topEval;
};
