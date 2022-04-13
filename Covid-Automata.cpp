#include <iostream>
#include "Covid-Automata.h"

#define N 10000
#define D 0.05
#define R 0.01
#define X 4
#define P1 40      // Probabilty (percentage) of people getting sick when not taking care
#define P2 10      // Probabilty (percentage) of people getting sick when taking care
#define T 0.1 * N      // P1 to P2 threshold

vector<Person*> personArr;

tuple<int, int> getRandomCoordinates() {
    srand(time(nullptr));
    int x;
    int y;
    while (true) {
        x = rand() % 199;
        y = rand() % 199;
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
    srand(time(nullptr));
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
        return (rand() % 100) < probability;
}



/**
 * Returns a random neighbor from neighbor matrix(including own position)
 * @param allNeighborCoor Vector containing tuples
 * @param selfCoord tuple representing current point's coordinates
 * @return new coordinates
 */
tuple<int,int> chooseRandomNeighbor(vector<tuple<int, int>> allNeighborCoor,
                                    tuple<int, int> selfCoord) {
    srand(time(nullptr));
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
 * Initialize matrix with sick, healthy and long distance cells
 */
void initialize() {
    int i = 0;
    int j = 0;
    int stop = N * D;
    int fast = N * R;
    // Initialize a random amount of sick and healthy long distance cells
    int fastHealthy = rand() % fast;
    int fastSick = fast - fastHealthy;
    // Loop initializing healthy
    for (i; i < stop; ++i) {
        auto coor = getRandomCoordinates();
        if (i > fastHealthy) {
            personArr.push_back(new HealthyPerson(coor, 0));
        } else {
            personArr.push_back(new HealthyPerson(coor, 9));
        }
    }
    stop = N - stop;
    sickCounter = stop;
    // Loop initializing sick
    auto coor = getRandomCoordinates();
    for (j; j < stop; ++j, ++i) {
        if (i > fastSick) {
            personArr.push_back(new SickPerson(coor, 0));
        } else {
            personArr.push_back(new SickPerson(coor, 9));
        }
    }
}

// Deletes person vector from memory
void clearMemory(vector<Person*> personArray) {
    for (auto person: personArray) {
        delete person;
    }
}


void execute() {
    initialize();
    int iteration = 20;
    int i = 0;
    int j = 0;
    for (i; i < iteration; ++i) {
        j = 0;
        for (auto person : personArr) {
            person -> nextIteration(j);
            ++j;
        }
    }
}


int main() {
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
    else if (x <= radius || x == 0) {
        output.push_back(tuple<int, int>(199 - radius + x, y));              // 2
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7

        // only top is wrapping
        if (y > radius && y < 199 - radius){
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, y + 1));  // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));    // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));    // 8
            return output;
        }

            //top and right are wrapping
        else if (y >= 199 - radius || y == 199) {
            output.push_back(tuple<int, int>(199 - radius + x, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(199 - radius + x, radius - (199 - y)));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));    // 4
            output.push_back(tuple<int, int>(x, radius - (199 - y)));        // 5
            output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x + 1 + radius, radius - (199 - y)));        // 8
            return output;
        }

            // top and left are wrapping.

        else if (y <= radius || y == 0) {
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
    else if (x >= 199 - radius || x == 199) {
        output.push_back(tuple<int, int>(x - 1 - radius, y));              // 2
        output.push_back(tuple<int, int>(199 - x + radius, y));                // 7

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
        else if (y >= 199 - radius) {
            output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));  // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));      // 3
            output.push_back(tuple<int, int>(x, y - 1 - radius));  // 4
            output.push_back(tuple<int, int>(x, y - 199 + radius));      // 5
            output.push_back(tuple<int, int>(x - 199 + radius, y - 1 - radius));    // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y - 199 + radius));        // 8
            return output;
        }

            // bottom left is wrapping
        else if (y <= radius) {
            output.push_back(tuple<int, int>(x - 1 - radius, 199 - y + radius));    // 1
            output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));  // 3
            output.push_back(tuple<int, int>(x, 199 - y + radius));    // 4
            output.push_back(tuple<int, int>(x, y + 1 + radius));  // 5
            output.push_back(tuple<int, int>(x - 199 + radius, 199 - y + radius));      // 6
            output.push_back(tuple<int, int>(x - 199 + radius, y + 1 + radius));        // 8
            return output;
        }

    }

    // only right is wrapping
    else if (y < radius || y == 0) {
        output.push_back(tuple<int, int>(x - 1 - radius, 199 - y + radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y + 1 + radius));   // 3
        output.push_back(tuple<int, int>(x, 199 - y + radius));             // 4
        output.push_back(tuple<int, int>(x, y + 1 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, 199 - y + radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y + 1 + radius));     // 8
        return output;
    }
    // only left is wrapping
    else if (y > 199 - radius || y == 199) {
        output.push_back(tuple<int, int>(x - 1 - radius, y - 1 - radius));      // 1
        output.push_back(tuple<int, int>(x - 1 - radius, y));           // 2
        output.push_back(tuple<int, int>(x - 1 - radius, y - 199 + radius));   // 3
        output.push_back(tuple<int, int>(x, y - 1 - radius));             // 4
        output.push_back(tuple<int, int>(x, y - 199 + radius));            // 5
        output.push_back(tuple<int, int>(x + 1 + radius, y - 1 - radius));      // 6
        output.push_back(tuple<int, int>(x + 1 + radius, y));            // 7
        output.push_back(tuple<int, int>(x + 1 + radius, y - 199 + radius));     // 8
        return output;
    }

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
