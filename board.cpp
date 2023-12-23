#include <ctime>
#include <cstdlib>
#include <QDebug>
#include "board.h"

Board::Board(int _size, int _difficulty)
{
    size = _size; // 4 5 6
    difficulty = _difficulty;
    numL2cars = 0;
    numL3cars = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data[i][j] = 0;
        }
    }
    nextStep = NULL;
    tail = NULL;
}


void Board::generateRandomCars() {
    numL2cars = 2 * difficulty + size - 3;
    numL3cars = difficulty + 2 * (size - 3);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data[i][j] = 0;
        }
    }

    // generate goal car:
    goalCar.column = size / 2;
    goalCar.row = 0;
    goalCar.length = 2;
    goalCar.ID = 99;
    goalCar.direction = 2; // Vertical

    // Place the goal car
    data[goalCar.row][goalCar.column] = goalCar.ID;
    data[goalCar.row + 1][goalCar.column] = goalCar.ID;


    // Place cars of length 2
    int i = 0;
    int cnt = 10000;
    while(cnt--) {
        Car car;
        car.length = 2;
        car.ID = i + 1; // IDs start from 1
        std::srand(time(0));
        // Randomly choose position and direction
        car.row = rand() % size;
        car.column = rand() % size;

        if (rand() % 2 == 0) { // 0 or 1 for horizontal or vertical
            car.direction = 1; // Horizontal
        } else {
            car.direction = 2; // Vertical
        }

        // Check if the chosen position is available
        bool positionAvailable = true;
        if (car.direction == 1 &&
            data[car.row][car.column] == 0 &&
            data[car.row][car.column + 1] == 0 &&
            car.column + 1 < size) { // Horizontal

            data[car.row][car.column] = car.ID;
            data[car.row][car.column + 1] = car.ID;
            L2cars[i] = car;
            i++;

        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   car.row + 1 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            L2cars[i] = car;
            i++;
        }
        if (i == numL2cars)
            break;
    }
    numL2cars = i;
    i = 0;
    cnt = 10000;
    while(cnt--) {
        Car car;
        car.length = 3;
        car.ID = i + 1 + numL2cars; // IDs start from 1
        std::srand(time(0));
        // Randomly choose position and direction
        car.row = cnt * rand() % size - 1;
        car.column = cnt * rand() % size - 1;

        if (rand() % 2 == 0) { // 0 or 1 for horizontal or vertical
            car.direction = 1; // Horizontal
        } else {
            car.direction = 2; // Vertical
        }

        // Check if the chosen position is available
        bool positionAvailable = true;
        if (car.direction == 1 &&
            data[car.row][car.column] == 0 &&
            data[car.row][car.column + 1] == 0 &&
            data[car.row][car.column + 2] == 0 &&
            car.column + 2 < size) { // Horizontal

            data[car.row][car.column] = car.ID;
            data[car.row][car.column + 1] = car.ID;
            data[car.row][car.column + 2] = car.ID;
            L3cars[i] = car;
            i++;

        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   data[car.row + 2][car.column] == 0 &&
                   car.row + 2 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            data[car.row + 2][car.column] = car.ID;
            L3cars[i] = car;
            i++;
        }
        if (i == numL3cars)
            break;
    }
    numL3cars = i;
    return;
}

bool Board::cleared(){
    bool flag = true;
    for (int i = goalCar.row + 2; i < size; i++){
        if (data[i][size / 2]  == 0)
            continue;
        else flag = false;
    }
    return flag;
}
bool Board::bfs(){

    // if find a answer
    return true;
}
void Board::update(){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data[i][j] = 0;
        }
    }

    data[goalCar.row][goalCar.column] = goalCar.ID;
    data[goalCar.row + 1][goalCar.column] = goalCar.ID;
    int carID = 0;
    for (int i = 0; i < numL2cars; i++) {
        carID++;
        Car* car = &L2cars[i];
        if (car->direction == 1) {
            for (int l = 0; l < car->length; l++){
                data[car->row][car->column + l] = car->ID;
            }
        }
        else {
            for (int l = 0; l < L2cars[i].length; l++){
                data[car->row + l][car->column] = car->ID;
            }
        }
    }
    for (int i = 0; i < numL3cars; i++) {
        carID++;
        Car* car = &L3cars[i];
        if (car->direction == 1) {
            for (int l = 0; l < car->length; l++){
                data[car->row][car->column + l] = car->ID;
            }
        }
        else {
            for (int l = 0; l < L2cars[i].length; l++){
                data[car->row + l][car->column] = car->ID;
            }
        }
    }
}
Car* Board::getCar(int x, int y){
    // return the pointer to the car locates in the given position.
    // return null if there is no car
    if (x < 0 || y < 0 || x >= size || y >= size) return nullptr;
    qDebug() << data[x][y];
    if (data[x][y] == 0) return nullptr;

    int carID = data[x][y];

    if (carID == goalCar.ID) return &goalCar;
    else if (carID <= numL2cars) return &L2cars[carID - 1];
    else return &L3cars[carID - numL2cars - 1]; // the name of variables are really terrible!!!! like shit
}
