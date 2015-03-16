#include "readCNF.h"

/*
 
 ****************************
 *         readCNF          *
 ****************************
 This program attempts to fulfill Project 1 for CS3445 at Bowdoin College.
 Daniel Cohen, Josh Imhoff, and Liam Taylor. 2015.
 
 This file contains code for reading in Conjunctive Normal Form (.cnf) files
 and storing their logical information. The format of the .cnf files is specific.
 
*/

// Global variables (see readCNF.h)
std::vector<std::vector<int>> clauses;
std::string filepath;

/*
 Reads in and stores specifically formatted (.cnf) conjunctive normal form problem information
 to evaluate the satisfiability of those closes in the MAXSAT problem
 Parameters:
        fp: the path of the file to be read in
 Returns the integer value representing the number of variables found in the problem
*/
int readClauses(std::string fp)
{
    filepath = fp;
    int maxVal = 0;
    
    std::fstream file(filepath);
    std::stringstream ss;
    
    if (file.is_open()) {
        std::vector<int> newClause;
        int val;
        std::string line;
        
        //For each clause in the file
        while (getline(file, line)) {
            
            ss.clear(); //clear fail flags
            ss.str(""); //empty stream
            ss << line; //read entire clause into stream
            
            // While there are still integers and a '0' has not been encountered,
            // build the new clause
            while (ss >> val && val != 0) {
                if (val > maxVal) maxVal = val;
                newClause.push_back(val);
            }
            
            // If a valid clause, add clause to SAT data
            if (!newClause.empty()) {
                clauses.push_back(newClause);
                newClause.clear();
            }
        }
        
    // Error checking
    } else {
        std::cout << "File did not open properly" << std::endl;
        exit(1);
    }
    
    file.close();
    
    // Return the number of variables found in the problem file
    return maxVal;
}


/*
 Prints the CNF clauses data after it has been read it, for testing purposes
*/
void printClauses(std::vector<std::vector<int>>& data)
{
    for (unsigned int i = 0; i < data.size(); i++) {
        std::cout << i << ": ";
        for (unsigned int j = 0; j < data[i].size(); j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
