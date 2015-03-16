CC = g++
CFLAGS= -g -Wall -std=c++0x

default: compile

compile: GeneticAlgorithm.o Sampler.o PBIL.o Individual.o readCNF.o
	$(CC) $? -o ga

GeneticAlgorithm.o: GeneticAlgorithm.cpp
	$(CC) $(CFLAGS) -c $< -o $@

Sampler.o: Sampler.cpp Sampler.h
	$(CC) $(CFLAGS) -c $< -o $@

Individual.o: Individual.cpp Individual.h
	$(CC) $(CFLAGS) -c $< -o $@

readCNF.o: readCNF.cpp readCNF.h
	$(CC) $(CFLAGS) -c $< -o $@

PBIL.o: PBIL.cpp PBIL.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ ga
