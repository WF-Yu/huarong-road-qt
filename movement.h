#ifndef MOVEMENT_H
#define MOVEMENT_H


class Movement
{
public:
    Movement(int _carID, int _direction) {
        carID = _carID;
        direction = _direction;
    }
    int carID;
    int direction;
    // int length = 1;
};

#endif // MOVEMENT_H
