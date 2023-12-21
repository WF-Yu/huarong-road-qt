#ifndef CAR_H
#define CAR_H


class Car
{
public:
    Car();
    Car(int, int, int, int, int);
    void move(int direction, int length);
    int getID() { return ID; }

    int row;
    int column;
    int length;
    int direction; // 1： in a row; 2: in a column
    int ID;  // for disolay and index
};

#endif // CAR_H
