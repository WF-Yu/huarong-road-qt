// mainwindow.cpp
#include "mainwindow.h"
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMouseEvent>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mainBoard(5, 1)
{
    // Create the main view
    view = new QGraphicsView(this);
    setCentralWidget(view);

    // Create a scene
    scene = new QGraphicsScene(this);
    view->setScene(scene);


    // Create the initial grid with default value.
    mainBoard.generateRandomCars();
    createGrid();


    // Generate menu
    generateMenu();
}

void MainWindow::createGrid()
{
    qDebug() << "draw grids";
    // Clear existing grid
    for (int i = 0; i < gridCells.size(); ++i) {
        for (int j = 0; j < gridCells[i].size(); ++j) {
            delete gridCells[i][j];
        }
    }

    gridCells.clear();

    // Calculate the size of each square
    int squareSize = 50;
    int N = mainBoard.size;
    // Create a new grid
    for (int i = 0; i < N; ++i) { // y
        QVector<QGraphicsRectItem*> row;
        for (int j = 0; j < N; ++j) { // x
            QGraphicsRectItem *rect = new QGraphicsRectItem(j * squareSize, i * squareSize, squareSize, squareSize);
            if (mainBoard.getCarID(j,i) == 99) {
                rect->setBrush(QColor(Qt::red));
            }
            else if (mainBoard.getCarID(j,i) > mainBoard.numL2cars)
                rect->setBrush(QColor(Qt::blue));
            else if (mainBoard.getCarID(j,i))
                rect->setBrush(QColor(Qt::yellow));
            else rect->setBrush(QColor(Qt::white));

            scene->addItem(rect);
            row.append(rect);
        }
        gridCells.append(row);
    }
}
void MainWindow::instructions(){
    // show instructions to users:


    // should be a flag here, and triggered every time when user click the board.
    // save current borad.
    // use change the board: generate new route.
}
void MainWindow::newGame()
{
    bool ok;
    // int gridSize;
    int newSize = QInputDialog::getInt(this, tr("New Grid Size"), tr("Enter the new grid size for this round:"), mainBoard.size, 1, 100, 1, &ok);

    // Choose difficulty
    QStringList difficultyOptions;
    difficultyOptions << tr("Easy") << tr("Medium") << tr("Hard");
    QString difficultyChosen = QInputDialog::getItem(this, tr("Choose Difficulty"), tr("Select difficulty:"), difficultyOptions, 0, false, &ok);

    // give options to user
    if (ok) {
        if (difficultyChosen == tr("Easy")) {
                mainBoard.difficulty = 1;
           } else if (difficultyChosen == tr("Medium")) {
                mainBoard.difficulty = 2;
           } else { // Hard
                mainBoard.difficulty = 3;
           }
        mainBoard.size = newSize;
        mainBoard.generateRandomCars();
        createGrid();
    }
}

void MainWindow::generateMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    // Menu option for new game
    QAction *CreateNewGame = new QAction(tr("New Game"), this);
    connect(CreateNewGame, &QAction::triggered, this, &MainWindow::newGame);
    fileMenu->addAction(CreateNewGame);
    QAction *showInstructions = new QAction(tr("Show Instructions"), this);
    connect(showInstructions, &QAction::triggered, this, &MainWindow::instructions);
    fileMenu->addAction(showInstructions);
    // Separator between "New Game" and "Exit"
    fileMenu->addSeparator();
    // Menu option for exit
    QAction *exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    fileMenu->addAction(exitAction);
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPos = view->mapToScene(event->pos());

    if (event->button() == Qt::LeftButton) {
        handleLeftMouseClick(clickPos);
    } else if (event->button() == Qt::RightButton) {
        handleRightMouseClick(clickPos);
    }

    QMainWindow::mousePressEvent(event);
}
/*
void MainWindow::handleMouseClick(const QPointF& clickPos)
{
    int squareSize = 50;
    qDebug() << clickPos.x() << ", " << clickPos.y();
    int clickedX = static_cast<int>(clickPos.x()) / squareSize;
    int clickedY = (static_cast<int>(clickPos.y()) - 30) / squareSize;

    qDebug() << "Clicked on square at (x, y): (" << clickedX << ", " << clickedY << ")";

    if (clickedX >= 0 && clickedX < mainBoard.size && clickedY >= 0 && clickedY < mainBoard.size) {
        QGraphicsRectItem* clickedRect = gridCells[clickedY][clickedX];
        // Perform actions with the clickedRect, e.g., change its color
        clickedRect->setBrush(QColor(Qt::green));
    }
}*/
void MainWindow::handleLeftMouseClick(const QPointF& clickPos)
{
    qDebug() << "left Click----------------------------------";
    // move car left or up
    int squareSize = 50;
    int clickedX = static_cast<int>(clickPos.x()) / squareSize;
    int clickedY = (static_cast<int>(clickPos.y()) - 30) / squareSize;
    Car* car = mainBoard.getCar(clickedX, clickedY);

    if (car != nullptr) {
        qDebug() << "returnd car id = " << car->ID;
        if (car->direction == 1) { // in a row, move up
            qDebug() << "direction = 1";
            if (car->column > 0 && mainBoard.data[car->row][car->column - 1] == 0) {
                qDebug() << "before, column = " << car->column;
                car->column -= 1;
                qDebug() << "column = " << car->column;
            }
        }
        else { // in a column, move left
            qDebug() << "direction = 2";
            if (car->row > 0 && mainBoard.data[car->row - 1][car->column] == 0) {
                qDebug() << "before, row = " << car->row;
                car->row -= 1;
                qDebug() << "row = " << car->row;
            }
        }
        mainBoard.update();
        createGrid(); // draw new graph
    }
}

void MainWindow::handleRightMouseClick(const QPointF& clickPos)
{
    // move car right or down
    qDebug() << "right Click---------------------";
    int squareSize = 50;
    int clickedX = static_cast<int>(clickPos.x()) / squareSize;
    int clickedY = (static_cast<int>(clickPos.y()) - 30) / squareSize;
    Car* car = mainBoard.getCar(clickedX, clickedY);
    if (car != nullptr) {
        qDebug() << "returnd car id = " << car->ID;
        if (car->direction == 1) { // in a row, move down.
            qDebug() << "direction = 1";
            if (car->column + car->length < mainBoard.size &&
                mainBoard.data[car->row][car->column + car->length] == 0)
            {
                qDebug() << "before, column = " << car->column;
                car->column++;
                qDebug() << "column = " << car->column;
            }
        }
        else { // in a column, move right.
           qDebug() << "direction = 2";
           if (car->row + car->length < mainBoard.size &&
               mainBoard.data[car->row + car->length][car->column] == 0) {
                qDebug() << "before, row = " << car->row;
               car->row++;
                qDebug() << "row = " << car->row;
           }
        }
        mainBoard.update();
        createGrid(); // draw new graph
    }

}


