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

tuple<int, int> getRandomCoordinates();


class Person {
public:
    tuple<int, int> location;
    int speed;
    Person(tuple<int, int> coor, int speed): location(coor), speed(speed){};
    std::vector<std::tuple<int, int>> getNeighbours(int radius);
    virtual void nextIteration(int index)=0;
    virtual ~Person() = default;
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
    ~HealthyPerson() override = default;

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
        sickCounter++;
    };
    SickPerson(tuple<int, int> coor, int speed, int gen): Person(coor, speed), generation(gen) {
        int x = get<0>(coor);
        int y = get<1>(coor);
        adjMat[x][y].person = this;
        adjMat[x][y].isOccupied = true;
        adjMat[x][y].isSick = true;
    };
    void nextIteration(int index) override;
    ~SickPerson() override = default;
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
    ~ImmunePerson() override = default;

};


#endif //BIOINFORMATICS1_COVID_AUTOMATA_H
