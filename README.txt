*******************************************************************************
Evolutionary Algorithms for MAXSAT:
A Comparison of Genetic Algorithms and Population Based Learning
*******************************************************************************
Dan Cohen, Josh Imhoff, and Liam Taylor
February 19, 2015
Project 1
CS3445

This project is designed to produce solutions to MAXSAT problems. Running the genetic algorithm will produce a MAXSAT solution developed by assembling a population of individuals, selecting individuals for breeding, crossing over offspring, and mutating for a number of generations. Running the PBIL algorithm will produce a MAXSAT solution by generating a probability vector, generating solution individuals based on that vector, assessing those individuals, then updating the probability vector towards the best solution and away from the worst. If either algorithm finds a solution that satisfies all the clauses in the MAXSAT problem, it will stop immediately and display solution information. If not, it will finish the given number of iterations and display the best solution found through the entire process.

TO RUN:
After running the makefile in the project folder directory, run the program from the command line using the following syntax:

“./ga”

then,

*****************
GENETIC ALGORITHM:
*****************
	* fileName: The file path of a .cnf MAXSAT problem file
	* numIndividuals: The number of individuals in an evolutionary generation
	* selectionMethod:
			“r” - Ranking Selection
			“t” - Tournament Selection
			“b” - Boltzmann Selection
	* crossoverMethod:
			“o” - One-point crossover
			“u” - Uniform crossover
	* crossoverProbability: The probability that the individuals will crossover while breeding
	* mutationProbability: The probability that the offspring will mutate after being bred
	* numGenerations: The number of generations to run
	* “g”

For example, to run GA with good default parameters on an example .cnf file, enter:
“./ga example.cnf 100 t o 0.7 0.01 1000 g”

****
PBIL:
****
	* fileName: The file path of a .cnf MAXSAT problem file
	* numIndividuals: The number of individuals generated in an iteration
	* positiveLearningRate: The rate at which the probability vector will update towards the best solution
	* negativeLearningRate: The rate at which the probability vector will update away from the worst solution
	* mutationProbability: The probability an index in the probability vector will be mutated
	* mutationShift: The magnitude that index will be mutated
	* numIterations: The number of iterations over which to produce individuals and update the probability vector
	* “p”

For example, to run PBIL with good default parameters on an example .cnf file, enter:
“./ga example.cnf 100 0.1 0.075 0.02 0.05 1000 p”					
