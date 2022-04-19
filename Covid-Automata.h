

#ifndef BIOINFORMATICS1_COVID_AUTOMATA_H
#define BIOINFORMATICS1_COVID_AUTOMATA_H
#include <random>    /* srand, rand */
#include <time.h>       /* time */
#include <utility>
#include <vector>
#include <tuple>
#include <sstream>
#include <stdlib.h>
#include <string>
#if defined _WIN32
#include "windows.h"
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
#endif
#include <unistd.h>
#include <fstream>

using namespace std;

class Cell;

struct matrixData {
    Cell* cell = nullptr;
    bool isOccupied = false;
    bool isHealthy = false;
    bool isSick = false;
    bool isImmune = false;
};

matrixData adjMat[200][200];

tuple<int, int> getRandomCoordinates();


class Cell {
public:
    tuple<int, int> location;
    int speed;
    Cell(tuple<int, int> coor, int speed): location(coor), speed(speed){};
    std::vector<std::tuple<int, int>> getNeighbours(int radius);
    virtual void nextIteration(int index)=0;
};

class HealthyCell : public Cell {
public:
    HealthyCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].cell = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isHealthy = true;
    };
    void nextIteration(int index) override;

};

class SickCell : public Cell{
    int generation = 1;
public:
    SickCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].cell = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isSick = true;
    };
    SickCell(tuple<int, int> coor, int speed, int gen): Cell(coor, speed), generation(gen) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].cell = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isSick = true;
    };
    void nextIteration(int index) override;
};

class ImmuneCell : public Cell {
public:
    ImmuneCell(tuple<int, int> coor, int speed): Cell(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].cell = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isImmune = true;
    };
    void nextIteration(int index) override;

};

// Output functions
void clear();
void printMatrix(matrixData adjMat[200][200]);


#endif //BIOINFORMATICS1_COVID_AUTOMATA_H
