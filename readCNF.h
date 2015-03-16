#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

/*
 
 ****************************
 *         readCNF          *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 See readCNF.cpp for more documentation. 
*/

// Globals set by readClauses. MAXSAT individuals need to access clauses, while
// printing in the algorithms must display the filepath. This global access helps
// keep the algorithms problem independent.
extern std::vector<std::vector<int>> clauses;
extern std::string filepath;

int readClauses(std::string filepath_);
void printClauses(std::vector<std::vector<int>>& data);

