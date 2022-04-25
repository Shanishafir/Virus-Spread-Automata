#include <iostream>
#include "Covid-Automata.h"




int N; //10000
double D = 0.05; //0.05
double R = 0.01; //0.01
int X = 4; //4
int P1 = 40; //40      // Probability (percentage) of people getting sick when not taking care
int P2 = 10; //10      // Probability (percentage) of people getting sick when taking care
int T; // 0.1 * N      // P1 to P2 threshold

int sickNumberTemp;    // Will store mid-iteration sick number of each iteration.
int sickCounter = 0;
int healthyCounter = 0;
int immuneCounter = 0;

std::random_device rd;

vector<Cell*> cellArr; // vector that will store pointers to all occupied cells.

/**
 * Checks for free cells on the grid, returns a random (x, y) coordinate as a tuple.
 * @return
 */
tuple<int, int> getRandomCoordinates() {
    int x;
    int y;
    std::uniform_int_distribution<int> distribution(0, 199);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
    while (true) {
        x = distribution(engine);
        y = distribution(engine);
        if (!adjMat[x][y].isOccupied) {
            break;
        }
    }
    return std::tuple<int, int>(x, y);
}

/*
 * Rolls whether a cell is sick in the next iteration
 * Uses
 */
bool amISick(Cell* cell) {
    // Create a uniform distribution number generator
    std::uniform_int_distribution<int> distribution(1, 100);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
    int value=distribution(engine);

    auto neighbors = cell->getNeighbours(0);
    int probability = 0;
    // Sick amount is below threshold
    if (sickCounter < T) {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adjMat[x][y].isOccupied && adjMat[x][y].isSick) {
                probability += P1;
            }
        }
    } else {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adjMat[x][y].isOccupied && adjMat[x][y].isSick) {
                probability += P2;
            }
        }
    }
    if (probability >= 100)
        return true;
    else
        return value < probability; // will be sick probability/100 times.
}

/**
 * This function ensures that each cell will only travel to an unoccupied cell in the following generation.
 * Selects a random neighbor coordinate from the surrounding cells(either neighbours with distance of 1, or neighbors
 * with distance of 10, depending on the cell, including the cell itself.)
 * Will select a cell from non occupied cells only
 * @param allNeighborCoor Vector containing tuples
 * @param selfCoord tuple representing current point's coordinates
 * @return new coordinates
 */
tuple<int,int> chooseRandomNeighbor(vector<tuple<int, int>> allNeighborCoor,
                                    tuple<int, int> selfCoord) {
    // Create a uniform distribution number generator


    vector<tuple<int, int>> freeSpaces;
    freeSpaces.push_back(selfCoord);
    int i = 1;
    for (tuple<int, int> neighborCoor : allNeighborCoor) {
        int x = get<0>(neighborCoor);
        int y = get<1>(neighborCoor);
        if (!adjMat[x][y].isOccupied) {
            ++i; // holds number of free cells
            freeSpaces.push_back(neighborCoor);
        }
    }
    std::uniform_int_distribution<int> distribution(0, i - 1);
    std::mt19937 engine(rd()); // Mersenne twister MT19937
    int choice = distribution(engine);
    return freeSpaces.at(choice);
}

/**
 * Clears all the flags of the requested cell.
 * @param x row number
 * @param y column number
 */
void freeCell(int x, int y) {
    adjMat[x][y].isOccupied = false;
    adjMat[x][y].isSick = false;
    adjMat[x][y].isHealthy = false;
    adjMat[x][y].isImmune = false;
    adjMat[x][y].cell = nullptr;
}

/**
 * Read and initialize automata parameters from user
 */
void getParams() {
    cout << "=========================================Viral Spread Automata=========================================" <<
    endl << endl << endl <<
    "\033[1;41;37mNOTE: it is highly recommended to set the terminal font to 5 or lower, and the terminal size to at least 201x201 after parameter setup,\n"
                    "in order to better see the whole simulation.\033[0m"  << endl << endl;

    while(true) {
        cout << "Enter desired number of cells, maximum of 40000 [Default = 30000]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            N = 30000;
            break;
        } else if (!(std::cin >> N) || N > 40000 || N < 2) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter fraction of sick cells, number between 0.0-1.0 [Default = 0.002]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            D = 0.002;
            break;
        } else if (!(std::cin >> D) || (D > 1.0 || D < 0.0)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of fast moving cells, value should be between 0.0-1.0 [Default = 0.01]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            R = 0.01;
            break;
        } else if (!(std::cin >> R) || (R > 1.0 || R < 0.0)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of generations until a sick cell stops infecting [Default = 2]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            X = 2;
            break;
        } else if (!(std::cin >> X) || (X < 1)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter probability of infection in percents(1 - 100) when number of sick cells is low [Default = 60]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            P1 = 60;
            break;
        } else if (!(std::cin >> P1) || (P1 < 1 || P1 > 100)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter probability of infection in percents(1 - 100) when number of sick high [Default = 5]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            P2 = 5;
            break;
        } else if (!(std::cin >> P2) || (P2 < 1 || P2 > 100)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    double x;
    while(true) {
        cout << "Enter sick cell threshold before cell infection percentage drops, values should be between 0.0-1.0 [Default = 0.1]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(100, '\n');
            T = 0.1 * N;
            break;
        } else if (!(std::cin >> x) || (x < 0.0 || x > 1.0)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            T = x * N;
            cin.clear();
            cin.ignore(100, '\n');
            break;
        }
    }
    cout << endl << "Parameters initialized! press Enter to start the simulation.";
    if (cin.peek() == '\n') {
        cin.clear();
        cin.ignore(100, '\n');
        return;
    }
}

/**
 * Initialize matrix with sick, healthy and long distance cells
 */
void initialize() {
    int i = 0;
    int j = 0;
    int numHealthy =  N - N * D;
    int fast = N * R;
    // Initialize a random amount of sick and healthy long distance cells
    int fastHealthy = rand() % fast;
    int fastSick = fast - fastHealthy;
    // Loop initializing healthy
    for (i; i < numHealthy; ++i) {
        auto coor = getRandomCoordinates();
        if (i > fastHealthy) {
            cellArr.push_back(new HealthyCell(coor, 0));
        } else {
            cellArr.push_back(new HealthyCell(coor, 9));
        }
    }
    healthyCounter = numHealthy; // initial number of healthy
    int numSick = N - numHealthy;
    sickCounter = numSick; // initial counter of sick
    // Loop initializing sick
    for (j; j < numSick; ++j) {
        auto coor = getRandomCoordinates();
        if (j > fastSick) {
            cellArr.push_back(new SickCell(coor, 0));
        } else {
            cellArr.push_back(new SickCell(coor, 9));
        }
    }
}

// Deletes cell vector from memory
void clearMemory(vector<Cell*> cellArray) {
    for (Cell* cell: cellArray) {
        delete cell;
    }
}

void execute() {
    clear();
    getParams();
    initialize();
    int iteration = 1000;
    int i = 0;
    int j = 0;
    std::ofstream outputFile("results.csv");
    outputFile << "Healthy,Sick,Immune" << endl;
    for (i; i < iteration; ++i) {
        j = 0;
        clear();
        cout << "\033[3;43;30mIteration\033[0m ";
        printf("\033[3;43;30m%d\033[0m\n", i);
        printMatrix(adjMat);
        sickNumberTemp = sickCounter;

        //cout << healthyCounter << "," << sickCounter << "," << immuneCounter << endl;
        outputFile << healthyCounter << "," << sickCounter << "," << immuneCounter << endl;

        if (sickCounter > 0) {
            // Iterate over each and every cell in the cell vector, advancing one cell at a time,
            // and ensuring that there will be no cell overlapping.
            for (auto cell : cellArr) {
                cell -> nextIteration(j);
                ++j;
            }
        } else
            break;
        sickCounter = sickNumberTemp; // update sickCounter with new value
    }
    outputFile.close();
    cout << endl << "Results were written to results.csv" << endl;
}


int main() {
    srand(time(NULL));
    execute();
    clearMemory(cellArr);
}


/*****************************************************************************
 ****************************** Object Functions *****************************
 *****************************************************************************/

/**
 * Returns a vector containing neighbor coordinates for the current cell
 * @param radius proximity to neighbors
 * @return tuple
 */
std::vector<std::tuple<int, int>> Cell::getNeighbours(int radius) {
    std::vector<std::tuple<int, int>> output;
    int x = get<0>(location);
    int y = get<1>(location);

    if (x > radius && y > radius && x < 199 - radius && y < 199 - radius) {
        output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));   // 3
        output.push_back(tuple<int, int>(x, y - 1 - radius));             // 4
        output.push_back(tuple<int, int>(x, y + 1 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));     // 8
        return output;
    }
    else if (x <= radius && x >= 0) {
        output.push_back(tuple<int, int>(199 - radius + x, y));              // 2
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7

        // only top is wrapping
        if (y > radius && y < 199 - radius){
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, y + 1 +radius));  // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));    // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));    // 8
            return output;
        }

        //top and right are wrapping
        else if (y >= 199 - radius && y < 200) {
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, radius - (199 - y)));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, radius - (199 - y)));        // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, radius - (199 - y)));        // 8
            return output;
        }

            // top and left are wrapping.

        else if (y <= radius && y >= 0) {
            output.push_back(tuple<int, int>(199 - radius + x, 199 - radius + y));    // 1
            output.push_back(tuple<int, int>(199 - radius + x, y + 1 + radius ));      // 3
            output.push_back(tuple<int, int>(x, 199 - radius + y));      // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));        // 5
            output.push_back(tuple<int, int>(x + 1 + radius, 199 - radius + y));      // 6
            output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));        // 8
            return output;
        }
    }
    // bottom is wrapping
    else if (x >= 199 - radius && x < 200) {
        output.push_back(tuple<int, int>(x - 1 - radius, y));              // 2
        output.push_back(tuple<int, int>(x - 199 + radius, y));                // 7

        // only bottom row is wrapping
        if (y > radius && y < 199 - radius) {
            output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));  // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));    // 5
            output.push_back(tuple<int, int>(x - 199 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y + 1 + radius));    // 8
            return output;
        }

        //bottom right is wrapping
        else if (y >= 199 - radius && y < 200) {
            output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));  // 4
            output.push_back(tuple<int, int>(x, y - 199 + radius));      // 5
            output.push_back(tuple<int, int>(x - 199 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y - 199 + radius));        // 8
            return output;
        }

            // bottom left is wrapping
        else if (y <= radius && y >= 0) {
            output.push_back(tuple<int, int>(x - 1 - radius, 199 - radius + y));    // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));  // 3
            output.push_back(tuple<int, int>(x, 199 - radius + y));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));  // 5
            output.push_back(tuple<int, int>(x - 199 + radius, 199 - radius + y));      // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y + 1 + radius));        // 8
            return output;
        }
    }
    // only left is wrapping
    else if (y <= radius  && y >= 0) {
        output.push_back(tuple<int, int>(x - 1 - radius, 199 - radius + y));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));   // 3
        output.push_back(tuple<int, int>(x, 199 - radius + y));             // 4
        output.push_back(tuple<int, int>(x, y + 1 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, 199 - radius + y));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));     // 8
        return output;
    }
    // only right is wrapping
    else if (y >= 199 - radius && y < 200) {
        output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));   // 3
        output.push_back(tuple<int, int>(x, y - 1 - radius));             // 4
        output.push_back(tuple<int, int>(x, y - 199 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y - 199 + radius));     // 8
        return output;
    } else
        output.push_back(getRandomCoordinates());
    return output;
}


/**
 * Performs one iteration step for the healthy cell that was called from.
 * @param index
 */
void HealthyCell::nextIteration(int index) {
    int x = get<0>(location);
    int y = get<1>(location);

    bool sickNextIteration = amISick(this);
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);

    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    // create a new Healthy cell
    if (!sickNextIteration) {
        cellArr.at(index) = new HealthyCell(newSpace, speed);
    }
    // Create a new sick cell
    else {
        healthyCounter--;
        sickNumberTemp++;
        cellArr.at(index) = new SickCell(newSpace, speed);
    }
    delete this;
}

void SickCell::nextIteration(int index) {
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    if (generation < X) {
        cellArr.at(index) = new SickCell(newSpace, speed, generation + 1);
    } else {
        cellArr.at(index) = new ImmuneCell(newSpace, speed);
        sickNumberTemp--;
        immuneCounter++;
    }
    delete this;
}

void ImmuneCell::nextIteration(int index) {
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    cellArr.at(index) = new ImmuneCell(newSpace, speed);
    delete this;
}



/****************************************************************************
 ****************************** Console Output ******************************
 ****************************************************************************/

/**
 * Clears the console display
 */
void clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequence
#elif defined (__APPLE__)
    system("clear");
#endif
}


void printMatrix(matrixData adjMat[200][200]) {
    int i = 0, j = 0;
    //iterate over rows
    for (;i < 200; ++i) {
        //iterate over columns
        for (j = 0; j < 200; ++j) {
            if (!adjMat[i][j].isOccupied)
                cout <<"_";
            else if (adjMat[i][j].isOccupied && adjMat[i][j].isHealthy)
               cout <<"\033[1;47;35mH\033[0m";
            else if (adjMat[i][j].isOccupied && adjMat[i][j].isSick)
                cout <<"\033[1;41;37mS\033[0m";
            else if (adjMat[i][j].isOccupied && adjMat[i][j].isImmune)
                cout << "\033[1;104;30mI\033[0m";
        }
        cout << endl;
    }
    //leave matrix on the screen for a couple of seconds
}
