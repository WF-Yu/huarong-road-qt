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
    // copy previous information. Instead of copying all the things, a constructor for car should be better :(
    size = _origin.size;
    difficulty = _origin.difficulty;
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
    qDebug() << "Board::Board(Board _origin, Movement move) {: building new board now";
    if (move.carID <= numL2cars) {
        // move a L2 car
        car = &L2cars[move.carID - 1];
    } else car = &L3cars[move.carID - 1 - numL2cars];
    car->printCar();
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
    this->printBoard();
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
void Board::clearBoard() {
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
}
void Board::generateRandomCars() {
    qDebug()<<"Entered generateRandomCars";
    std::srand(time(0));

    // maybe we read from File directly......seems random figure generation is not working correctly.
do {
    clearBoard();
    qDebug() << "in generate random cars: try again";
    // Place cars of length 2
    int i = 0;
    int cnt = 10000;
    numL2cars = difficulty + size - 3;
    numL3cars = difficulty;
    while(cnt--) {
        // qDebug() << "-------    while(cnt--) { ------------";
        Car car;
        car.length = 2;
        car.ID = i + 1; // IDs start from 1
        // Randomly choose position and direction
        car.row = rand() % (size - 1);
        car.column = rand() % (size - 1);

        if (rand() % 2 == 0) { // 0 or 1 for horizontal or vertical
            car.direction = 1; // Horizontal
        } else {
            car.direction = 2; // Vertical
        }

        // Check if the chosen position is available
        bool positionAvailable = true;
        if (car.direction == 1) {
            if (
            data[car.row][car.column] == 0 &&
            data[car.row][car.column + 1] == 0 &&
            car.column + 1 < size) { // Horizontal

                data[car.row][car.column] = car.ID;
                data[car.row][car.column + 1] = car.ID;
                L2cars[i] = car;
                i+=1;
                qDebug() << "generated a car:";
                car.printCar();
                qDebug() << "i = " << i;
            }
        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   car.row + 1 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            L2cars[i] = car;
            i+=1;
            qDebug() << "generated a car:";
            car.printCar();
            qDebug() << "i = " << i;

        }
        if (i == numL2cars)
            break;
    } //     while(cnt--) {

    numL2cars = i;
    qDebug() << "Generated " << numL2cars << "L2 cars";
    i = 0;
    cnt = 10000;
    while(cnt--) {
        // qDebug() << "-------    while(cnt--) { ------------";

        Car car;
        car.length = 3;
        car.ID = i + 1 + numL2cars; // IDs start from 1
        // std::srand(time(0));
        // Randomly choose position and direction
        car.row = cnt * rand() % (size - 2);
        car.column = cnt * rand() % (size - 2);

        if (rand() % 2 == 0) { // 0 or 1 for horizontal or vertical
            car.direction = 1; // Horizontal
        } else {
            car.direction = 2; // Vertical
        }

        // Check if the chosen position is available
        bool positionAvailable = true;
        if (car.direction == 1){
            if (
            data[car.row][car.column] == 0 &&
            data[car.row][car.column + 1] == 0 &&
            data[car.row][car.column + 2] == 0 &&
            car.column + 2 < size) { // Horizontal

                data[car.row][car.column] = car.ID;
                data[car.row][car.column + 1] = car.ID;
                data[car.row][car.column + 2] = car.ID;
                L3cars[i] = car;
                i+=1;
                qDebug() << "generated a car:";
                car.printCar();
                qDebug() << "Added this car to board";
                printBoard();
                qDebug() << "i = " << i;


            }
        } else if (data[car.row][car.column] == 0 &&
                   data[car.row + 1][car.column] == 0 &&
                   data[car.row + 2][car.column] == 0 &&
                   car.row + 2 < size) { // Vertical
            data[car.row][car.column] = car.ID;
            data[car.row + 1][car.column] = car.ID;
            data[car.row + 2][car.column] = car.ID;
            L3cars[i] = car;
            i+=1;
            qDebug() << "generated a car:";
            car.printCar();
            qDebug() << "i = " << i;

        }
        if (i == numL3cars)
            break;
    }
    numL3cars = i;
    qDebug() << "Generated " << numL3cars << "L3 cars";

    qDebug() << "Generated Board and let's check if it is valid!";
    printBoard();
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
    bool flag = false;
    qDebug() << "enter bfs now";
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
        qDebug() << "entered while loop";


        bool ifNewNode = false;
        Board* board = head->node->board;
        board->printBoard();
        // add children into queue
        // for each child, we need to check a)if it is used before(compare with nodes in usedQueue)
        //                                  b)if it is a solution
        for (int i = 0; i < board->numL2cars + board->numL3cars ; i++) {
            qDebug() << "entered for loop, get a car:";

            // for each car:
            Car* car = nullptr;
            if (i < board->numL2cars ) car = &board->L2cars[i];
            else car = &board->L3cars[i - numL2cars];

            car->printCar();

            if (((car->direction == 1) && (car->column - 1 >= 0)) ||
                    ((car->direction == 2) && (car->row - 1 >= 0))) // if not out of boundary
                if (((car->direction == 1) && (board->data[car->row][car->column - 1] == 0)) ||
                        ((car->direction == 2) && (board->data[car->row - 1][car->column] == 0)))  // if avaliable
                { // move up or left

                    Movement* move = new Movement(car->ID, 1);
qDebug() << "before apply movement:";
board->printBoard();
                    // generate new Board with this movement.
qDebug() << "after  apply movement:";
                    Board* newBoard = new Board(*board, *move);
                    // if the new board is the same as previous board, continue

                    if (usedQHead != nullptr)
                        if (!usedQHead->ifUsed(*newBoard))
                            goto l1;

                    // check if this is a solution:
                    if (newBoard->cleared()) {
                        // do something to store this solution!!!!!!!!!!!!!!!!!!!
                        qDebug() << "find the solution!";
                        flag = true;
                        goto leave;
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
l1:
            if (((car->direction == 1) && (car->column + car->length < board->size)) ||
                ((car->direction == 2) && (car->row + car->length < board->size)))
                if (((car->direction == 1) && (board->data[car->row][car->column + car->length] == 0)) ||
                        ((car->direction == 2) && (board->data[car->row + car->length][car->column] == 0)))
                { // move down or right

                    Movement* move = new Movement(car->ID, 2);

                    // generate new Board with this movement.
                    Board* newBoard = new Board(*board, *move);
                    // if the new board is the same as previous board, continue
                    if (usedQHead != nullptr)
                        if (!usedQHead->ifUsed(*newBoard))
                            goto l2;

                    // check if this is a solution:
                    if (newBoard->cleared()) {
                        // do something to store this solution!
                        qDebug() << "find the solution!";

                        flag = true;
                        goto leave;
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
l2:
        // add the movement of goal car into queue also:
        if (goalCar.row > 0)
            if (board->data[goalCar.row - 1][goalCar.column] == 0)
            {
                Movement* move = new Movement(goalCar.ID, 1);

                // generate new Board with this movement.
                Board* newBoard = new Board(*board, *move);
                // if the new board is the same as previous board, continue
                if (usedQHead != nullptr)
                    if (!usedQHead->ifUsed(*newBoard))
                        goto l3;

                // check if this is a solution:
                if (newBoard->cleared()) {
                    // do something to store this solution!
                    qDebug() << "find the solution!";

                    flag = true;
                    goto leave;
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
    l3:
        if (goalCar.row + 2 < size)
            if (board->data[goalCar.row + 1][goalCar.column] == 0) {
                Movement* move = new Movement(goalCar.ID, 2);

                // generate new Board with this movement.
                Board* newBoard = new Board(*board, *move);
                // if the new board is the same as previous board, continue
                if (usedQHead != nullptr)
                    if (!usedQHead->ifUsed(*newBoard)) // if used: return false
                        goto l4;

                // check if this is a solution:
                if (newBoard->cleared()) {
                    // do something to store this solution!
                    qDebug() << "find the solution!";

                    flag = true;
                    goto leave;
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
l4:
        if (!ifNewNode) break;
        // move head into usedQueue.
        QueueNode* temp = head->next;
        if (usedQHead == nullptr) {
            usedQHead = head;
            usedQTail = head;
            usedQTail->next = nullptr;
        }
        else {
            usedQTail->next = head;
            usedQTail = usedQTail->next;
            usedQTail->next = nullptr;
        }
        head = temp;
        if (head == nullptr) goto leave; // no solutions. actually program will not come to here.
    } // while(1)

    // delete all created items:
leave:
    QueueNode* temp;
    while(head != nullptr) {
        temp = head->next;
        delete head;
        head = temp;

    }
    while(usedQHead != nullptr) {
        temp = usedQHead->next;
        delete usedQHead;
        usedQHead = temp;
    }
    qDebug() << "leaving bfs now, flag: " << flag;
    return flag;
}


void Board::update(){ // put car information into data[][].
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            data[i][j] = 0;
        }
    }

    data[goalCar.row][goalCar.column] = goalCar.ID;
    data[goalCar.row + 1][goalCar.column] = goalCar.ID;
    for (int i = 0; i < numL2cars; i++) {
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
    if (cleared()) return false;
    return bfs();
    // return true;
}

void Board::printBoard()  {
    qDebug() << "--------------------------------";
    for (int i = 0; i < size; i++) {
        qDebug() << data[0][i] << ", " << data[1][i] << ", " <<
                    data[2][i] << ", " << data[3][i] << ", " <<
                    data[4][i] << ", " << data[5][i];
    }
    qDebug() << "numL2cars: " << numL2cars;
    qDebug() << "numL3cars: " << numL3cars;

    qDebug() << "--------------------------------";

}
