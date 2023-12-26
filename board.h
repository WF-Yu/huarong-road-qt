#ifndef BOARD_H
#define BOARD_H
#include "car.h"
#include "movement.h"
class Board
{
public:
    Board(int, int);
    Board(Board*);
    // ~Board();
    Board(Board, Movement);
    bool equal(Board another); // return true if two boards are the same.
    bool valid(); // check if current layout is valid and has a answer.
    void generateRandomCars(); // put cars into board based on difficulty and gridsize
    bool bfs();
    bool cleared(); // ?? I forget how to use it!
    int getCarID(int x, int y) { return data[x][y]; }
    Car* getCar(int x, int y);
    void update(); // called after user moved car.
    void clearBoard();

    // for test
    void printBoard();

    int size; // 4*4 5*5 6*6
    int difficulty;
    int data[6][6]; // 0 if no cars. ID if there is a car
    Car goalCar;
    Car L2cars[6]; // length = 2
    Car L3cars[6]; // length = 3
    int numL2cars; // number of cars
    int numL3cars;
 };

#endif // BOARD_H
