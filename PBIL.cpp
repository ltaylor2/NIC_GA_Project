#include "PBIL.h"

/*
 
 ****************************
 *          PBIL            *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 The PBIL class contains code to initialize and run a population based incremental
 learning algorithm. The class is designed to run on a generalized problem, with 
 problem-specifics defined using Individual subclasses (see Individual.cpp). A probability
 vector produces a population of solutions which are assessed in a problem-specific way,
 and the probability vector is then updated towards the best solution and away from the worst,
 with possible mutation to add variation. When a complete solution is achieved, or at the end
 of the algorithm's iterations, the best solution is displayed.
 
*/

/*
 Constructor for the population based incremental learning algorithm, which automatically initializes
 member values and runs the PBIL process when constructed
 Parameters:
        probVectSize: Problem-specific size of the probability vector for initialization
        sampleIndividual: Problem-specific indivudal of the correct type for abstract individual generation
        numIndividuals: Number of individuals to be produced from the probVect in a single interation
        posLR: Learning rate at which to move the probability vector towards the best solution
        negLR: Learning rate at which to move the probability away from the worst solution
        mutationProb: Probability at which a probability in the probVect will be mutated
        mutationShift: Proportional amount that the probVect will be mutated at an index
        numIter: Number of interations to run the algorithm
        maxSolution_: Maximum soltution for the abstract problem (different than the best POSSIBLE solution)
*/
PBIL::PBIL(int probVectSize,
           std::shared_ptr<Individual> sampleIndividual,
           int numIndividuals,
           double posLR, double negLR,
           double mutationProb, double mutationShift,
           int numIter,
           double maxSolution_)
    : maxSolution(maxSolution_)
{
    //init the probability vector
    for (int i = 0; i < probVectSize; i++) {
        probVect.push_back(0.5);
    }
    
    // Actually run the PBIL algorithm
	run(sampleIndividual, numIndividuals, posLR, negLR, mutationProb, mutationShift, numIter);
}

/*
 Performs population based incremental learning, producing individuals based on a probability 
 vector, assessing those generations of individuals, then shifting the probability vector towards
 the best solution of that generation in order to generate better solution.
 Parameters:
        sampleIndividual: Problem-specific indivudal of the correct type for abstract individual generation
        numIndividuals: Number of individuals to be produced from the probVect in a single interation
        posLR: Learning rate at which to move the probability vector towards the best solution
        negLR: Learning rate at which to move the probability away from the worst solution
        mutationProb: Probability at which a probability in the probVect will be mutated
        mutationShift: Proportional amount that the probVect will be mutated at an index
        numIter: Number of interations to run the algorithm
*/
void PBIL::run(std::shared_ptr<Individual> sampleIndividual,
               int numIndividuals, double posLR, double negLR,
               double mutationProb, double mutationShift, int numIter)
{
    // Initialize the top vars to avoid accidentally pointing
    // to random data
	currIter = 0;
    topIter = 0;
    topEval = 0.0;
    topSolution = NULL;
    success = false;
    
	while (++currIter <= numIter) {
        
        // Generate the appropriate number of individuals
        // evaluate all of their solutions, and mark the best and worst
        for (int i = 0; i < numIndividuals; i++) {
            population.push_back(sampleIndividual->generateIndividualPBIL(probVect));
            evaluations.push_back(population[i]->evaluateFitness());
        }

		// Find the best and worst solutions (the lowest fitness),
		// update the probVect, and mutate the probVect
        findBestAndWorst();
        updateProbVect(posLR, negLR);
        mutateProbVect(mutationProb, mutationShift);
        
        
        // If there is the maximum possible fitness given the (generic) problem,
        // end the algorithm and display that best individual
        if (topEval == maxSolution) {
            printSuccess();
            success = true;
            break;
        }
        
        std::cout << "Iteration: " << currIter << std::endl;
        
        // Clear the pop and evals for the next generation
        population.clear();
        evaluations.clear();
    }
    
    // If the iterations have finished, display the best solution found yet
    if (!success)
        printEnd();
}

/*
 Finds the highest and lowest fitness of the current probability-generated population
 and keeps track of the highest fitness individual of any generation found yet
*/
void PBIL::findBestAndWorst()
{
	bestEvalIndex = 0;
	worstEvalIndex = 0;
    
    // Search all the evaluations to find the best (highest fit)
    // and worst (lowest fit) individuals
	for (unsigned int i = 0; i < evaluations.size(); i++) {
		if (evaluations[i] >= evaluations[bestEvalIndex]) 
			bestEvalIndex = i;
			
		if (evaluations[i] <= evaluations[worstEvalIndex])
			worstEvalIndex = i;
    }
    
    // Population and evaluations share indeces
	bestSolution = population[bestEvalIndex];
	worstSolution = population[worstEvalIndex];
    
    // Check if the best of this generation is the best of any generation
    if (evaluations[bestEvalIndex] > topEval) {
        topEval = evaluations[bestEvalIndex];
        topSolution = bestSolution;
        topIter = currIter;
    }
}

/*
 First updates the probability vector towards the best solution by shifting
 the values of the probVect towards the value of the solution at that index,
 then updates the probVect "away from the worst solution" by shifting the probVect
 towards the best solution (at a DIFFERENT RATE) if the best and worst solutions 
 have different values at that index
 Parameters:
        posLR: Magnitude shift in the probability vector towards the best solution
        negLR: Magnitude shift in the probability vector away from the worst solution
*/
void PBIL::updateProbVect(double posLR, double negLR)
{
    // Adjusts the value of the probVect towards the best solution at constricted
    // rate proportional to the positive learning rate
	for (unsigned int i = 0; i < probVect.size(); i++) {
		probVect[i] = probVect[i] * (1.0 - posLR) + bestSolution->at(i) * posLR;
	}

    // Adjusts the value of the probVect towards the best solution at a constricted
    // (different) rate, but only if the best solution and the worst solution are different,
    // therefore moving the value further away from the worst solution than if it was shared
    // with the best
	for (unsigned int i = 0; i < probVect.size(); i++) {
		if (bestSolution->at(i) != worstSolution->at(i)) {
			probVect[i] = probVect[i] * (1.0 - negLR) + bestSolution->at(i) * negLR;
		}
	}
}

/*
 Mutate the probability vector by, with a certain chance, shifting it randomly up or down,
 although never mutating the probability towards 1 or 0
 Parameters:
        mutationProb: Probability at which a probability in the probVect will be mutated
        mutationShift: Proportional amount that the probVect will be mutated at an index
*/
void PBIL::mutateProbVect(double mutationProb, double mutationShift)
{
	for (unsigned int i = 0; i < probVect.size(); i++) {
		if ((double)rand() / RAND_MAX < mutationProb) {
            int mutationDirection = 0;
            
			// Mutate the probability at an index so it's never shifted all the way down to 0, or all
            // the way up to one (both of which would get the generation stuck until it mutated again)
            if ((double)rand() / RAND_MAX < 0.5)
                mutationDirection = 1;
            
            probVect[i] = probVect[i] * (1.0 - mutationShift)
                          + mutationDirection * mutationShift;
		}
	}
}

/*
 Prints information regarding a 100% solution found before the specified number of
 generations were complete. The print functions are, unfortunately,
 not completely generalized
*/
void PBIL::printSuccess() const
{
    std::cout << std::endl
              << "For file: " << filepath << std::endl
              << "With " << probVect.size() << " variables and " << maxSolution << " clauses" << std::endl
              << "Complete solution found in iteration: " << currIter << std::endl
              << topEval << " out of " << maxSolution << " clauses satisfied: "
                         << topEval/maxSolution * 100 << "%" << std::endl
              << "Assignment: " << std::endl;
    
    topSolution->print();
}

/*
 Prints information regarding the best solution found, in any generation, after the
 specified number of generations were completed. The print functions are, unfortunately,
 not completely generalized
*/
void PBIL::printEnd() const
{
    std::cout << std::endl
              << "Iterations Complete." << std::endl
              << "For file: " << filepath << std::endl
              << "With " << probVect.size() << " variables and " << maxSolution << " clauses" << std::endl
              << "Best solution found in iteration: " << topIter << std::endl
              << topEval << " out of " << maxSolution << " clauses satisfied: "
                         << topEval/maxSolution * 100 << "%" << std::endl
              << "Assignment: " << std::endl;
    
    // Only print the individual if there is one, avoids segfaults on 0 generations
    if (topSolution)
        topSolution->print();
    else
        std::cout << "No assignment found." << std::endl;
}


