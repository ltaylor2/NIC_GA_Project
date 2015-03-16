#pragma once

#include "Individual.h"
#include "Sampler.h"

#include <memory>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <ctime>

/*
 
 ****************************
 *     GeneticAlgorithm     *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 See GeneticAlgorithm.cpp for class and method documentation.
 
*/

// The abstract GA, written to evolve generations of Individuals.
class GeneticAlgorithm {
public:
    GeneticAlgorithm(std::vector<std::shared_ptr<Individual>> initialPopulation,
                     SelectionMethod selectionMethod, 
                     CrossoverMethod crossoverMethod,
                     double crossoverProb,
                     double mutationProb,
                     int numOfGenerations,
                     double maxSolution_,
                     int numVars_);
    
    std::vector<std::shared_ptr<Individual>>& getFinalPopulation() { return population; }

private:
    void run(SelectionMethod selectionMethod, 
             CrossoverMethod crossoverMethod,
             double crossoverProb,
             double mutationProb,
             int numOfGenerations);
    
    int getBestIndividual() const;

    void printSuccess() const;
    void printEnd() const;
    
    int currentGeneration;

    // Vectors to store the abstract individuals and the evaluations
    // of those individuals
    std::vector<std::shared_ptr<Individual>> population;
    std::vector<double> evaluations;
    
    double maxSolution;
    int numVars;
    
    int bestFitIndex;
    
    // Info to keep track of the best individual found over all of the
    // generations runs
    double topFitness;
    int topGeneration;
    std::shared_ptr<Individual> topIndividual;
    
    bool success;
};
