//
// Created by shani on 11/04/2022.
//

#ifndef BIOINFORMATICS1_COVID_AUTOMATA_H
#define BIOINFORMATICS1_COVID_AUTOMATA_H
#include <random>    /* srand, rand */
#include <time.h>       /* time */
#include <utility>
#include <vector>
#include <tuple>
#include <sstream>

using namespace std;

class Person;

struct matrixData{
    Person* person = nullptr;
    bool isOccupied = false;
    bool isHealthy = false;
    bool isSick = false;
    bool isImmune = false;
};

matrixData adjMat[200][200];
int sickCounter = 0;
int healthyCounter = 0;
int immuneCounter = 0;
tuple<int, int> getRandomCoordinates();


class Person {
public:
    tuple<int, int> location;
    int speed;
    Person(tuple<int, int> coor, int speed): location(coor), speed(speed){};
    std::vector<std::tuple<int, int>> getNeighbours(int radius);
    virtual void nextIteration(int index)=0;
};

class HealthyPerson : public Person {
public:
    HealthyPerson(tuple<int, int> coor, int speed): Person(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].person = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isHealthy = true;
    };
    void nextIteration(int index) override;

};

class SickPerson : public Person{
    int generation = 1;
public:
    SickPerson(tuple<int, int> coor, int speed): Person(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].person = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isSick = true;
    };
    SickPerson(tuple<int, int> coor, int speed, int gen): Person(coor, speed), generation(gen) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].person = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isSick = true;
    };
    void nextIteration(int index) override;
};

class ImmunePerson : public Person {
public:
    ImmunePerson(tuple<int, int> coor, int speed): Person(coor, speed) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].person = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isImmune = true;
    };
    void nextIteration(int index) override;

};


#endif //BIOINFORMATICS1_COVID_AUTOMATA_H
