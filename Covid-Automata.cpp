#include <iostream>
#include "Covid-Automata.h"

int N; //10000
double D = 0.05; //0.05
double R = 0.01; //0.01
int X = 4; //4
int P1 = 40; //40      // Probabilty (percentage) of people getting sick when not taking care
int P2 = 10; //10      // Probabilty (percentage) of people getting sick when taking care
int T; // 0.1 * N      // P1 to P2 threshold

vector<Person*> personArr;

tuple<int, int> getRandomCoordinates() {
    int x;
    int y;
    while (true) {
        x = rand() % 200;
        y = rand() % 200;
        if (!adjMat[x][y].isOccupied) {
            break;
        }
    }
    return std::tuple<int, int>(x, y);
}

/*
 * Rolls whether a person is sick in the next iteration
 */
bool amISick(Person* person) {
    std::random_device rd;
    std::uniform_int_distribution<int> distribution(1, 100);
    std::mt19937 engine(rd()); // Mersenne twister MT19937

    int value=distribution(engine);

    auto neighbors = person->getNeighbours(0);
    int probability = 0;
    // Sick amount is below threshold
    if (sickCounter < T) {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adjMat[x][y].isSick) {
                probability += P1;
            }
        }
    } else {
        for (auto neighbor : neighbors) {
            int x = get<0>(neighbor);
            int y = get<1>(neighbor);
            if (adjMat[x][y].isSick) {
                probability += P2;
            }
        }
    }
    if (probability >= 100)
        return true;
    else
        return value < probability;
}

/**
 * Returns a random neighbor from neighbor matrix(including own position)
 * @param allNeighborCoor Vector containing tuples
 * @param selfCoord tuple representing current point's coordinates
 * @return new coordinates
 */
tuple<int,int> chooseRandomNeighbor(vector<tuple<int, int>> allNeighborCoor,
                                    tuple<int, int> selfCoord) {
    vector<tuple<int, int>> freeSpaces;
    freeSpaces.push_back(selfCoord);
    int i = 1;
    for (tuple<int, int> neighborCoor : allNeighborCoor) {
        int x = get<0>(neighborCoor);
        int y = get<1>(neighborCoor);
        if (!adjMat[x][y].isOccupied) {
            ++i;
            freeSpaces.push_back(neighborCoor);
        }
    }
    // choose a new coordinate.
    int choice = rand() % i;
    return freeSpaces.at(choice);
}

void freeCell(int x, int y) {
    adjMat[x][y].isOccupied = false;
    adjMat[x][y].isSick = false;
    adjMat[x][y].isHealthy = false;
    adjMat[x][y].isImmune = false;
    adjMat[x][y].person = nullptr;
}

/**
 * Read and initialize automata parameters from user
 */
void getParams() {
    cout << "====Viral Spread Automata====" << endl << endl;
    while(true) {
        cout << "Enter desired number of cells, maximum of 40000 [Default = 15000]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            N = 15000;
            break;
        } else if (!(std::cin >> N) || N > 40000 || N < 2) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter fraction of sick cells, number between 0.0-1.0 [Default = 0.05]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            D = 0.05;
            break;
        } else if (!(std::cin >> D) || (D > 1.0 || D < 0.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of fast moving cells, value should be between 0.0-1.0 [Default = 0.01]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            R = 0.01;
            break;
        } else if (!(std::cin >> R) || (R > 1.0 || R < 0.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter number of generation until a sick cell stops infecting[Default = 4]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            X = 4;
            break;
        } else if (!(std::cin >> X) || (X < 1)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter percentage(1 - 100) of getting sick when number of sick cells is low [Default = 40]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            P1 = 40;
            break;
        } else if (!(std::cin >> P1) || (P1 < 1 || P1 > 100)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    while(true) {
        cout << "Enter percentage(1 - 100) of getting sick when number of sick cells is high [Default = 10]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            P2 = 10;
            break;
        } else if (!(std::cin >> P2) || (P2 < 1 || P2 > 100)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    double x;
    while(true) {
        cout << "Enter sick cell threshold before cell infection percentage drops, values should be between 0.0-1.0 [Default = 0.1]: ";
        //check if next character is newline
        if (cin.peek() == '\n') {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            T = 0.1 * N;
            break;
        } else if (!(std::cin >> x) || (x < 0.0 || x > 1.0)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input, please try again." << endl;     //error handling
        } else {
            T = x * N;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
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
            personArr.push_back(new HealthyPerson(coor, 0));
        } else {
            personArr.push_back(new HealthyPerson(coor, 9));
        }
    }
    healthyCounter = numHealthy; // initial number of healthy
    int numSick = N - numHealthy;
    sickCounter = numSick; // initial counter of sick
    // Loop initializing sick
    auto coor = getRandomCoordinates();
    for (j; j < numSick; ++j) {
        if (j > fastSick) {
            personArr.push_back(new SickPerson(coor, 0));
        } else {
            personArr.push_back(new SickPerson(coor, 9));
        }
    }
}

// Deletes person vector from memory
void clearMemory(vector<Person*> personArray) {
    for (Person* person: personArray) {
        delete person;
    }
}

void execute() {
    getParams();
    initialize();
    int iteration = 1000;
    int i = 0;
    int j = 0;
    cout << "Healthy,Sick,Immune" << endl;
    for (i; i < iteration; ++i) {
        j = 0;
        cout << healthyCounter << "," << sickCounter << "," << immuneCounter << endl;
        for (auto person : personArr) {
            person -> nextIteration(j);
            ++j;
        }
    }
}


int main() {
    srand (time(NULL));

    execute();
    clearMemory(personArr);
}

/**
 * Returns the neighbor coordinates of the current person
 * @param radius proximity to neighbors
 * @return tuple
 */
std::vector<std::tuple<int, int>> Person::getNeighbours(int radius) {
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



void HealthyPerson::nextIteration(int index) {
    int x = get<0>(location);
    int y = get<1>(location);

    bool sickNextIteration = amISick(this);
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);

    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    // create a new Healthy cell
    if (!sickNextIteration) {
        personArr.at(index) = new HealthyPerson(newSpace, speed);
    }
    // Create a new sick cell
    else {
        healthyCounter--;
        sickCounter++;
        personArr.at(index) = new SickPerson(newSpace, speed);
    }
    delete this;
}

void SickPerson::nextIteration(int index) {
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    if (generation < X) {
        personArr.at(index) = new SickPerson(newSpace, speed, generation + 1);
    } else {
        personArr.at(index) = new ImmunePerson(newSpace, speed);
        sickCounter--;
        immuneCounter++;
    }
    delete this;
}

void ImmunePerson::nextIteration(int index) {
    auto allNeighborCoor = getNeighbours(speed);
    auto newSpace = chooseRandomNeighbor(allNeighborCoor, location);
    int x = get<0>(location);
    int y = get<1>(location);
    // Freeing up current location, so that another cell can occupy it.
    freeCell(x, y);
    personArr.at(index) = new ImmunePerson(newSpace, speed);
    delete this;
}
