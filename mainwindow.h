#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QComboBox>
#include "board.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void handleLeftMouseClick(const QPointF& clickPos);
    void handleRightMouseClick(const QPointF& clickPos);
    bool ifShowInstructions;
private slots:
    void newGame();
    void instructions();
private:
    void createGrid();
    void generateMenu();

    Board mainBoard; // board to show

    QGraphicsScene *scene;
    QGraphicsView *view;
    QVector<QVector<QGraphicsRectItem*>> gridCells;
    QComboBox *difficultyComboBox; // Added ComboBox for difficulty selection
};

#endif // MAINWINDOW_H
