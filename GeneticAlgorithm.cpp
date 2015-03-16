#include "GeneticAlgorithm.h"
#include "readCNF.h"
#include "PBIL.h"

/*
 
 ****************************
 *     GeneticAlgorithm     *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 This file includes the main function.
 
 The GeneticAlgorithm class contains code to initialize and run an evolutionary based
 algorithm to solve an abstract problem. The code works in conjunction with Individuals
 (see Individual.cpp) to remain completely general. Individuals in a population are assesed
 for their fitness levels, are selected for breeding based on those scores, and then
 potentially crossed-over or mutated to add variation. Crossover and selection occur in the
 Sampler class. Other functions help in printing that information to view the best solution.
 
*/

/*
 Constructor for the Genetic Algorithm, which automatically intializes member values and
 runs the algorithm process when constructed
 Parameters:
      initialPopulation: Vector of pointers to abstract individuals that serve as the first generation
      selectionMethod: Parsed info to send to Sampler in order to specify GA selection
      crossoverMethod: Parsed info in order to specify GA crossover during breeding
      crossoverProb: Probability that individuals will crossover during breeding
      mutationProb: Probability that a bred individual's information will be mutated
      numOfGenerations: Number of iterations the GA will run
      maxSolution_: Maximum solution for the abstract problem (different than the best POSSIBLE solution)
      numVars_: The number of variables in the problem
*/
GeneticAlgorithm::GeneticAlgorithm(std::vector<std::shared_ptr<Individual>> initialPopulation,
                                   SelectionMethod selectionMethod, 
                                   CrossoverMethod crossoverMethod,
                                   double crossoverProb,
                                   double mutationProb,
                                   int numOfGenerations,
                                   double maxSolution_,
                                   int numVars_)
: currentGeneration(0), population(initialPopulation), maxSolution(maxSolution_), numVars(numVars_)
{
    // Actually run the Genetic Algorithm
    run(selectionMethod, crossoverMethod, crossoverProb,
        mutationProb, numOfGenerations);
}

/*
 Performs the fitness-based generation of a population of abstract individuals,
 breeding selected individuals for the next generation and potentially crossing over and
 mutating the offspring in order to generate better solutions from the population
 Parameters:
      selectionMethod: Parsed info to send to Sampler in order to specify GA selection
      crossoverMethod: Parsed info in order to specify GA crossover during breeding
      crossoverProb: Probability that indiviudals will crossover during breeding
      mutationProb: Probability that a bred individual's information will be mutated
      numOfGenerations: Number of iterations the GA will run
*/
void GeneticAlgorithm::run(SelectionMethod selectionMethod,
                          CrossoverMethod crossoverMethod,
                          double crossoverProb,
                          double mutationProb,
                          int numOfGenerations)
{
    // Initialize the top vars to avoid accidentally pointing
    // to random data
    topFitness = 0.0;
    topGeneration = 0;
    topIndividual = NULL;
    success = false;
    
    while (++currentGeneration <= numOfGenerations) {
        
        // Make sure the evaluations from the old populations are gone
        // and get the new evaluations (so they don't have to be calculated
        // on the fly in different selection methods)
        evaluations.clear();
        for(unsigned int i = 0; i < population.size(); i++) {
            evaluations.push_back(population[i]->evaluateFitness());
        }
        
        // Get the best individual from the population
        bestFitIndex = getBestIndividual();
        
        // Assess the best EVER individual of all generations and store it
        // in case the maximum solution isn't achieved
        if (evaluations[bestFitIndex] > topFitness) {
            topFitness = evaluations[bestFitIndex];
            topIndividual = population[bestFitIndex];
            topGeneration = currentGeneration;
        }
        
        // If there is the maximum possible fitness given the (generic) problem,
        // end the algorithm and display that best individual
        if (topFitness == maxSolution) {
            printSuccess();
            success = true;
            break;
        }
        
        // Make the vector for the next generation and construct
        // the sampler with the supplied selection method
        std::vector<std::shared_ptr<Individual>> nextGeneration;
        Sampler sampler(selectionMethod, population, evaluations);

        // Breed the current generation and send the individuals to the next generation
        while (nextGeneration.size() != population.size()) {
            std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> offspring;

            // With probability crossoverProb, breed two Individuals
            if (static_cast<double>(rand()) / RAND_MAX < crossoverProb) {
                offspring = sampler.select()->breed(crossoverMethod, *(sampler.select()));
            } else { // Else, just take an Individual
                offspring.first = sampler.select();
            }

            // With probability mutationProb, mutate the Individual (or pair of individuals)
            offspring.first->mutate(mutationProb);
            if (offspring.second) {
                offspring.second->mutate(mutationProb);
            }

            // Add individual (or pair) to the next generation
            nextGeneration.push_back(offspring.first);
            if (offspring.second && nextGeneration.size() < population.size()) {
                nextGeneration.push_back(offspring.second);
            }
        }

        // Move the std::move is efficient, as it avoids copying the entire vector
        population = std::move(nextGeneration);
        
        std::cout << "Generation: " << currentGeneration << std::endl;
    }
    
    // If the generations have finished, display the best solution found yet
    if (!success)
        printEnd();
}

/*
 Finds the "best" (the highest fitness) individual in the current population
 Returns the index of the that best individual (for both eval and pop vectors)
*/
int GeneticAlgorithm::getBestIndividual() const
{
    int bestFitIndex = 0;
    double bestFit = 0.0;

    // Finds and saves the highest fitness individual
    for (unsigned int i = 0; i < population.size(); i++) {
        if (evaluations[i] > bestFit) {
            bestFitIndex = i;
            bestFit = evaluations[i];
        }
    }

    // The individual will have the same index in evaluations and population vectors
    return bestFitIndex;
}

/*
 Prints information regarding a 100% solution found before the specified number of
 generations were completed. The print functions are, unfortunately,
 not completely generalized
*/
void GeneticAlgorithm::printSuccess() const
{
    std::cout << std::endl
              << "For file: " << filepath << std::endl
              << "With " << numVars << " variables and " << maxSolution << " clauses" << std::endl
              << "Complete solution found in generation: " << currentGeneration << std::endl
              << topFitness << " out of " << maxSolution << " clauses satisfied: "
                            << topFitness/maxSolution * 100 << "%" << std::endl
              << "Assignment: " << std::endl;

    topIndividual->print();
}

/*
 Prints information regarding the best solution found, in any generation, after the
 specified number of generations were completed. The print functions are, unfortunately,
 not completely generalized
*/
void GeneticAlgorithm::printEnd() const
{
    std::cout << std::endl
              << "Generations Complete." << std::endl
              << "For file: " << filepath << std::endl
              << "With " << numVars << " variables and " << maxSolution << " clauses" << std::endl
              << "Best solution found in generation: " << topGeneration << std::endl
              << topFitness << " out of " << maxSolution << " clauses satisfied: "
                            << topFitness/maxSolution * 100 << "%" << std::endl
              << "Assignment: " << std::endl;
    
    // Only print the individual if there is one, avoids segfaults on 0 generations
    if (topIndividual)
        topIndividual->print();
    else
        std::cout << "No assignment found." << std:: endl;
}


/*                     
 The main function, which reads in arguments from the command line to specify running either
 a genetic algorithm or population based incremental learning algorithm, here implemented for
 MAXSAT problems
*/
int main(int argc, char** argv)
{
    // Check commandline arguments
    if (argc != 9) {
        std::cout << "USAGE GA: filename populationSize selectionMethod crossoverMethod crossoverProb mutationProb numGenerations g"
                  << std::endl << "USAGE PBIL: filename generationSize positiveLearningRate negativeLearningRate mutationProbability mutationShift numIterations p"
                  << std::endl;
        return -1;
    }

    // Read in problem
    int numVars = readClauses(argv[1]);
    
    // Seed random number generator
    // NOTE Done here since it must be done before building initial population
    //      but cannot be done more than once
    srand(time(NULL));
    
    // Time how long it takes to run algorithm
    std::clock_t start = clock();

    std::string algorithmInput(argv[8]);
    
    // Get ready to run the genetic algorithm by parsing the string
    // parameters and calling the GA constructor
    if (algorithmInput == "g") {
        
        // Parse selection method
        std::string selectionInput(argv[3]);
        SelectionMethod selectionMethod; 
        if (selectionInput == "r")
            selectionMethod = SelectionMethod::ranking;
        else if (selectionInput == "t")
            selectionMethod = SelectionMethod::tournament;
        else if (selectionInput == "b")
            selectionMethod = SelectionMethod::boltzmann;
        else {
            std::cout << "USAGE invalid selection method" << std::endl;
            return -1;
        }

        // Parse crossover method
        std::string crossoverInput(argv[4]);
        CrossoverMethod crossoverMethod; 
        if (crossoverInput == "o")
            crossoverMethod = CrossoverMethod::onePoint;
        else if (crossoverInput == "u")
            crossoverMethod = CrossoverMethod::uniform;
        else {
            std::cout << "USAGE invalid crossover method" << std::endl;
            return -1;
        }

        // Build initial population so the GA algorithm can run
        // on a generic population
        std::vector<std::shared_ptr<Individual>> initialPopulation;
        for (int i = 0; i < atoi(argv[2]); i++) {
            initialPopulation.push_back(std::shared_ptr<Individual>(new MAXSATIndividual(numVars)));
        }

        // Run genetic algorithm
        GeneticAlgorithm ga(initialPopulation, selectionMethod,
                            crossoverMethod, std::stod(std::string(argv[5])), 
                            std::stod(std::string(argv[6])), atoi(argv[7]),
                            clauses.size(), numVars);
    }
    
    // Get ready to run the PBIL algorithm by constructing a sample individual
    // and calling the PBIL constructor
    else if (algorithmInput == "p") {
        
        // Generate a sample individual of the desired type so the PBIL can
        // generically generate new individuals from that sample type
        std::shared_ptr<Individual> sample(new MAXSATIndividual(numVars));

        // Run PBIL
        PBIL pbil(numVars, sample, atof(argv[2]),
                  atof(argv[3]), atof(argv[4]), atof(argv[5]),
                  atof(argv[6]), atoi(argv[7]), clauses.size());
    }
      
    // Report no appropriate algorithm selection (g or p)
    else {
        std::cout << "USAGE invalid alogrithm" << std::endl;
        return -1;
    }
    
    // Calculate and output elapsed time
    std::clock_t end = clock();
    double timeInSeconds = (end - start) / (double) CLOCKS_PER_SEC;
    std::cout << "Time elapsed: " << timeInSeconds << " seconds" << std::endl;
    
    // Return OK status and close program
    return 0;
}
  
