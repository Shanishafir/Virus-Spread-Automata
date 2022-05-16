main:
	g++ -std=c++11 Covid-Automata.cpp Covid-Automata.h -o viral-automaton.out
clean:
	rm -f viral-automaton.out results.csv

