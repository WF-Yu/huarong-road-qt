#include <ctime>
#include <cstdlib>
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
            L2cars[car.ID] = car;
            i++;

        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   car.row + 1 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            L2cars[car.ID] = car;
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
            L3cars[car.ID] = car;
            i++;

        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   data[car.row + 2][car.column] == 0 &&
                   car.row + 2 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            data[car.row + 2][car.column] = car.ID;
            L3cars[car.ID] = car;
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
