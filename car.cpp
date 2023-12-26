#include "car.h"
#include <QDebug>
Car::Car(int row, int column, int length, int direction, int ID)
{

}
Car::Car() {

}
void Car::move(int direction, int length) {
// should use this one! in mainWindow.cpp.
}
void Car::printCar() {
    qDebug() << "car direction: " << direction;
    qDebug() << "car start position: (" << row << ", " << column << ")";
    qDebug() << "car length" << length;
    qDebug() << "car id = " << ID;
}
