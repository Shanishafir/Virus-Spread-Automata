main:
	g++ -std=c++11 Covid-Automaton.cpp Covid-Automaton.h -o viral-automaton.out
clean:
	rm -f viral-automaton.out results.csv

