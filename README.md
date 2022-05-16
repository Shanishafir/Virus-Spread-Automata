# Virus Spread Simulation Cellular Automaton
A program simulating a viral outbreak and resembling [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life/) written in C++.

The automaton allows for setting up various variables, such as starting number of cells, infection rates, fast travelling cells etc.

The program implements a simple CLI UI for graphical display of the simulation.

### Prerequisites
  - g++/c++ compiler
  - Linux OS, tested on kernel ver. 5.17.6.

### Running the program
   - Use `git clone` to receive the repository
   - A makefile was provided for conveniece, run `make` to compile the source, a viral-automaton.out file will appear.
   - In order to execute run `./viral-automaton.out` and follow the on screen instructions
      - After parameter initalization, it is highly reccommended to set the terminal window to 201 rows and 200 columns, in order to fully see the automation.
   - To delete the executable and the results file, run `make clean`

