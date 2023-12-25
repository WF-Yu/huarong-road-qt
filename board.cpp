#include <ctime>
#include <cstdlib>
#include <QDebug>
#include "board.h"
#include "treenode.h"
#include "queuenode.h"

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
}
Board::Board(Board _origin, Movement move) {
    /*
    int data[6][6]; // 0 if no cars. ID if there is a car
    Car goalCar;
    Car L2cars[6]; // length = 2
    Car L3cars[6]; // length = 3
    int numL2cars; // number of cars
    int numL3cars;
    */
    // copy previous information. Instead of copying all the things, a constructor for car should be better :(
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            data[i][j] = _origin.data[i][j];
        }
    }
    numL2cars = _origin.numL2cars;
    numL3cars = _origin.numL3cars;
    for (int i = 0; i < numL2cars; i++) {
        L2cars[i].ID = _origin.L2cars[i].ID;
        L2cars[i].column = _origin.L2cars[i].column;
        L2cars[i].direction = _origin.L2cars[i].direction;
        L2cars[i].row = _origin.L2cars[i].row;
        L2cars[i].length = _origin.L2cars[i].length;
    }
    for (int i = 0; i < numL3cars; i++) {
        L3cars[i].ID = _origin.L3cars[i].ID;
        L3cars[i].column = _origin.L3cars[i].column;
        L3cars[i].direction = _origin.L3cars[i].direction;
        L3cars[i].length = _origin.L3cars[i].length;
        L3cars[i].row = _origin.L3cars[i].row;

    }
    goalCar.ID = _origin.goalCar.ID;
    goalCar.column = _origin.goalCar.column;
    goalCar.direction = _origin.goalCar.direction;
    goalCar.row = _origin.goalCar.row;
    goalCar.length = _origin.goalCar.length;

    Car* car = nullptr;
    // add movement into the board:
    if (move.carID <= numL2cars) {
        // move a L2 car
        car = &L2cars[move.carID - 1];
    } else car = &L3cars[move.carID - 1 - numL2cars];
    if (move.direction == 1) { // move left or up
        if (car->direction == 1) { // move up
            car->column -= 1;
        }
        else { // move left
            car->row -= 1;
        }
    }
    else { // move right or down
        if (car->direction == 1) { // move down
            car->column += 1;
        }
        else { // move right
            car->row += 1;
        }
    }

    this->update();
}
bool Board::equal(Board _another) {
    /*
    int size; // 4*4 5*5 6*6
    int difficulty;
    int data[6][6]; // 0 if no cars. ID if there is a car
    Car goalCar;
    Car L2cars[6]; // length = 2
    Car L3cars[6]; // length = 3
    int numL2cars; // number of cars
    int numL3cars;
    */
    if (size != _another.size || difficulty != _another.difficulty) return false;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (data[i][j] != _another.data[i][j]) return false;
        }
    }
    return true;
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
    goalCar.direction = 2; // shui ping

    // Place the goal car
    data[goalCar.row][goalCar.column] = goalCar.ID;
    data[goalCar.row + 1][goalCar.column] = goalCar.ID;

do {
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

    // check if valid!
} while (!valid());
    return;
}

bool Board::cleared(){
    bool flag = true;
    for (int i = goalCar.row + 2; i < size; i++){
        if (data[i][goalCar.column]  == 0)
            continue;
        else flag = false;
    }
    return flag;
}

bool Board::bfs() {
    QueueNode* head = nullptr;
    QueueNode* tail = nullptr;

    // we need to make a copy of current board. the current board shouldn't be deleted. 
    // tree structure: children pointing to its parent. parent will not point to children because it has too many children
    TreeNode* currentNode = new TreeNode();
    Board* copiedMainboard = new Board(*this);
    currentNode->board = copiedMainboard;
    currentNode->parent = nullptr;
    currentNode->move = NULL;

    // store boards whose children have been added into queue
    QueueNode* usedQHead = nullptr;
    QueueNode* usedQTail = nullptr;

    // initialize
    head = new QueueNode();
    head->node = currentNode;
    head->next = nullptr;
    tail = head;

    while (1) { // while there are more children to be added into queue. if there is no more new child to add, it means there is no solution.

        bool ifNewNode = false;
        Board* board = head->node->board;

        // add children into queue
        // for each child, we need to check a)if it is used before(compare with nodes in usedQueue)
        //                                  b)if it is a solution
        for (int i = 0; i < board->numL2cars + board->numL3cars ; i++){
            // for each car:
            Car* car = nullptr;
            if (i < board->numL2cars) car = &board->L2cars[i];
            else car = &board->L3cars[i - numL2cars];

            // if (car->direction == 1) { // in a row
            if ((car->direction == 1) && (car->column - 1 >= 0) ||
                    (car->direction == 2) && (car->row - 1 >= 0))
            { // move up or left
                Movement* move = new Movement(car->ID, 1);

                // generate new Board with this movement.
                Board* newBoard = new Board(*board, *move);
                // if the new board is the same as previous board, continue

                if (usedQHead != nullptr)
                    if (!usedQHead->ifUsed(*newBoard))
                        continue;

                // check if this is a solution:
                if (newBoard->cleared()) {
                    // do something to store this solution!!!!!!!!!!!!!!!!!!!
                    return true;
                }
                // generate new treeNode
                TreeNode* newTreeNode = new TreeNode();
                newTreeNode->board = newBoard;
                newTreeNode->move = move;
                newTreeNode->parent = head->node;

                // generate new queue node based on this treenode and add it to queue:
                QueueNode* newQueuenode = new QueueNode();
                newQueuenode->node = newTreeNode;
                tail->next = newQueuenode;
                tail = tail->next;
                tail->next = nullptr;
                ifNewNode = true;
            }
            if ((car->direction == 1) && (car->column + car->length < board->size) ||
                (car->direction == 2) && (car->row + car->length < board->size)) { // move down or right
                Movement* move = new Movement(car->ID, 2);

                // generate new Board with this movement.
                Board* newBoard = new Board(*board, *move);
                // if the new board is the same as previous board, continue
                if (usedQHead != nullptr)
                    if (!usedQHead->ifUsed(*newBoard))
                        continue;

                // check if this is a solution:
                if (newBoard->cleared()) {
                    // do something to store this solution!
                    return true;
                }

                // generate new treeNode
                TreeNode* newTreeNode = new TreeNode();
                newTreeNode->board = newBoard;
                newTreeNode->move = move;
                newTreeNode->parent = head->node;

                // generate new queue node based on this treenode and add it to queue:
                QueueNode* newQueuenode = new QueueNode();
                newQueuenode->node = newTreeNode;
                tail->next = newQueuenode;
                tail = tail->next;
                tail->next = nullptr;
                ifNewNode = true;
            }

        }

        if (!ifNewNode) break;

        // move head into usedQueue.
        usedQTail->next = head;
        usedQTail = usedQTail->next;
        usedQTail->next = nullptr;
        head = head->next;
        // if (head == nullptr) return false; // no solutions. actually program will not come to here.
    }
    // delete all created items:
    QueueNode* temp = head->next;
    while(temp != nullptr) {
        delete head;
        head = temp;
        temp = head->next;
    }
    temp = usedQHead->next;
    while(temp != nullptr) {
        delete usedQHead;
        usedQHead = temp;
        temp = usedQHead->next;
    }
    return false;
}

void Board::update(){ // put car information into data[][].
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
            for (int l = 0; l < L3cars[i].length; l++){
                data[car->row + l][car->column] = car->ID;
            }
        }
    }
}
Car* Board::getCar(int x, int y){
    // return the pointer to the car locates in the given position.
    // return null if there is no car
    if (x < 0 || y < 0 || x >= size || y >= size) return nullptr;
    qDebug() << "in getCar: car ID = " << data[x][y];
    if (data[x][y] == 0) return nullptr;

    int carID = data[x][y];

    if (carID == goalCar.ID) return &goalCar;
    else if (carID <= numL2cars) return &L2cars[carID - 1];
    else return &L3cars[carID - numL2cars - 1]; // the name of variables are really terrible!!!! like shit
}
bool Board::valid(){
    // check is there is an answer.
    // return bfs();
    return true;
}
