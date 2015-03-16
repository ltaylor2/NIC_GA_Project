#pragma once

#include "readCNF.h"

#include <utility>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <vector>

/*
 
 ****************************
 *        Individual        *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 See Individual.cpp for more class documentation.
 
*/

enum class CrossoverMethod { onePoint, uniform };

// Abstract base class used to represent a candidate solution.
// Genetic algorithm and PBIL are written in terms of the Individual class, so that
// it can solve any problem so long as a suitable subclass of Individual has
// been written.
class Individual {
public:
    // Virtual destructor necessary to ensure proper object destruction
    virtual ~Individual() {}

    // Method that mutates the object
    virtual void mutate(double mutationProb) = 0;

    // Method that combines two Individuals to create a new Individual using
    // the crossover method specified.
    // std::shared__ptr is used, so that the Individual (created on the
    // heap) is deleted automatically
    virtual std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>> 
            breed(CrossoverMethod method, const Individual& mate) const = 0;

    // Returns a positive double representing the fitness of the Individual
    virtual double evaluateFitness() const = 0;

    // Clones the object for use in the Sampler class
    virtual std::shared_ptr<Individual> clone() const = 0;

    // Human readable print function used by the GA to display results
    virtual void print() const = 0;
    
    // Gets the data at an index of the individual dataset
    virtual int at(int index) const = 0;
    
    // Generates a new individual from a probability vector for PBIL
    virtual std::shared_ptr<Individual> generateIndividualPBIL(const std::vector<double>& probVect) = 0;
};

// An individual subclass to apply genetic and PBIL algorithms to MAXSAT problems.
// Class and method documentation can be found in Individual.cpp
class MAXSATIndividual : public Individual {
public:
    
	MAXSATIndividual(int size);
	MAXSATIndividual(std::vector<bool> data_);
    MAXSATIndividual(const std::vector<double>& probVect);
    
    void mutate(double mutationProb);
    std::pair<std::shared_ptr<Individual>, std::shared_ptr<Individual>>
            breed(CrossoverMethod method, const Individual& mate) const;
    double evaluateFitness() const;
    void print() const;
    std::shared_ptr<Individual> clone() const;
    
    int at(int index) const;
    std::shared_ptr<Individual> generateIndividualPBIL(const std::vector<double>& probVect);
    
    
private:
    std::vector<bool> data;
};