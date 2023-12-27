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
#include <QCoreApplication>
#include <QtMultimedia/QMediaPlaylist>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mainBoard(5, 1)
{
    // 设置窗口的初始大小为固定值
    resize(800, 800);
    // Create the main view
    view = new QGraphicsView(this);
    setCentralWidget(view);

    // Create a scene
    scene = new QGraphicsScene(this);
    view->setScene(scene);

    ifShowInstructions = false;

    qDebug() << "hi";
    // Create the initial grid with default value.
    mainBoard.generateRandomCars();
    keyBoardRow = 0;
    keyBoardColumn = 0;
    createGrid();

   // 设置背景音乐文件路径
   // QString musicFilePath = "F:/MyProject/Qt_projects/huarong-road-qt/LOL.mp3";
    playBackgroundMusic();
    // Generate menu
    generateMenu();

    generatePromptMsg();

}
void MainWindow::generatePromptMsg() {
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget());

    // Add a spacer item to the right to create some space
    // centralLayout->addItem(new QSpacerItem(200, 400, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Create and add the label to the layout
    QLabel *label = new QLabel(
        "汽车华容道：将目标车右移，移动到窗口最右侧（当目标车右侧没有车辆阻拦时，即为通关）\n"
        "红色：目标车\n"
        "黄色：长度为2的车\n"
        "蓝色：长度为3的车\n"
        "鼠标操作：左键（左移或上移），右键（右移或下移）\n"
        "键盘操作：\n"
        "    绿色：键盘当前位置\n"
        "    wsad键：上下左右\n"
        "    ctrl+wsad：将键盘所在位置的车向上下左右方向移动\n"
        , this);

    label->setAlignment(Qt::AlignLeft); // 设置对齐方式，根据需要调整
    centralLayout->addWidget(label);

    // Set the layout for the central widget
    centralWidget()->setLayout(centralLayout);

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

    int instructedCarID = 0;
    int instructedDirection = 0;
    int rectRow = -1;
    int rectColumn = -1;
    if (ifShowInstructions) {
        // get the instruction: which car to click, and which direction
        mainBoard.getInstructions(&instructedCarID, &instructedDirection);
        Car* car = nullptr;
        if (instructedCarID != -1) {
            if (instructedCarID <= mainBoard.numL2cars) {
            // L2 car
                car = &mainBoard.L2cars[instructedCarID - 1];
            }
            else {
               // L3 car
               car = &mainBoard.L2cars[instructedCarID - mainBoard.numL2cars - 1];
            }

           if (car->direction == 1){
               if (instructedDirection == 1){ // move up
                    rectRow = car->row;
                    rectColumn = car->column;
               }
               else { // move down
                   rectRow = car->row;
                   rectColumn = car->column + car->length - 1;
               }
           }
           else {
               if (instructedDirection == 1){ // move left
                   rectRow = car->row;
                   rectColumn = car->column;
               }
               else { // move right
                   rectRow = car->row + car->length - 1;
                   rectColumn = car->column;
               }
           }
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
            if (j == rectRow && i == rectColumn) {
                // instructions: which rect to click next
                rect->setBrush(QColor(Qt::black));
            }
            else if (j == keyBoardRow && i == keyBoardColumn) {
                // current keyboard position
                rect->setBrush(QColor(Qt::green));
            }
            else if (mainBoard.getCarID(j,i) == 99) {
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

// start a new game
void MainWindow::newGame()
{
    ifShowInstructions = false;
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
        // generate cars for the new game
        mainBoard.generateRandomCars();
        createGrid();
    }
}

void MainWindow::instructions() {
    // tell user how to move the cars
    ifShowInstructions = true;
    createGrid();
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

void MainWindow::keyPressEvent(QKeyEvent *event) {
   if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A) {
       qDebug() << "Ctrl+A Pressed";
       handKeyPress_CtrlA();
   }
   else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W) {
       qDebug() << "Ctrl+W Pressed";
       handKeyPress_CtrlW();
   }
   else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_D) {
       qDebug() << "Ctrl+D Pressed";
       handKeyPress_CtrlD();
   }
   else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
       qDebug() << "Ctrl+S Pressed";
       handKeyPress_CtrlS();
   }
   else if (event->key() == Qt::Key_A) {
       qDebug() << "A Pressed";
       handKeyPress_A();
   }
   else if (event->key() == Qt::Key_W) {
       qDebug() << "W Pressed";
       handKeyPress_W();
   }
   else if (event->key() == Qt::Key_D) {
       qDebug() << "D Pressed";
       handKeyPress_D();
   }
   else if (event->key() == Qt::Key_S) {
       qDebug() << "S Pressed";
       handKeyPress_S();
   }
   // 调用基类的函数以确保其他键盘事件正常处理
   QWidget::keyPressEvent(event);
}
void MainWindow::handKeyPress_CtrlA() { // move to left
    Car* car = mainBoard.getCar(keyBoardRow, keyBoardColumn);
    if (car == nullptr) return;
    if (car->direction == 2) {
        if (car->row > 0) {
            car->row -= 1;
        }
    }
    mainBoard.update(); // update new car position to board
    if (mainBoard.cleared()) {
        // succeed!
        ShowSuccessInfo();
        ifShowInstructions = false;
    }
    createGrid(); // draw new graph

}
void MainWindow::handKeyPress_CtrlW() { // move up
    Car* car = mainBoard.getCar(keyBoardRow, keyBoardColumn);
    if (car == nullptr) return;

    if (car->direction == 1) {
        if (car->column > 0) {
            car->column -= 1;
        }
    }
    mainBoard.update(); // update new car position to board
    if (mainBoard.cleared()) {
        // succeed!
        ShowSuccessInfo();
        ifShowInstructions = false;
    }
    createGrid(); // draw new graph
}
void MainWindow::handKeyPress_CtrlD() { // move left
    Car* car = mainBoard.getCar(keyBoardRow, keyBoardColumn);
    if (car == nullptr) return;

    if (car->direction == 2) {
        if (car->row + car->length < mainBoard.size) {
            car->row += 1;
        }
    }
    mainBoard.update(); // update new car position to board
    if (mainBoard.cleared()) {
        // succeed!
        ShowSuccessInfo();
        ifShowInstructions = false;
    }
    createGrid(); // draw new graph
}
void MainWindow::handKeyPress_CtrlS() { // move down
    Car* car = mainBoard.getCar(keyBoardRow, keyBoardColumn);
    if (car == nullptr) return;

    if (car->direction == 1) {
        if (car->column + car->length < mainBoard.size) {
            car->column += 1;
        }
    }
    mainBoard.update(); // update new car position to board
    if (mainBoard.cleared()) {
        // succeed!
        ShowSuccessInfo();
        ifShowInstructions = false;
    }
    createGrid(); // draw new graph
}
void MainWindow::handKeyPress_A() { // move left
    if (keyBoardRow > 0) {
        keyBoardRow -= 1;
        createGrid(); // draw new graph
    }
}
void MainWindow::handKeyPress_W() { // move up
    if (keyBoardColumn > 0) {
        keyBoardColumn -= 1;
        createGrid(); // draw new graph
    }
}
void MainWindow::handKeyPress_D() { // move right
    if (keyBoardRow < mainBoard.size - 1) {
        keyBoardRow += 1;
        createGrid(); // draw new graph
    }
}
void MainWindow::handKeyPress_S() { // move down
    if (keyBoardColumn < mainBoard.size - 1) {
        keyBoardColumn += 1;
        createGrid(); // draw new graph
    }
}
void MainWindow::handleLeftMouseClick(const QPointF& clickPos)
{

    qDebug() << "left Click----------------------------------";
    // move car left or up
    int squareSize = 50;
    int clickedX = static_cast<int>(clickPos.x()) / squareSize;
    int clickedY = (static_cast<int>(clickPos.y()) - 30) / squareSize;
    setKeyboard(clickedX, clickedY);

    Car* car = mainBoard.getCar(clickedX, clickedY);

    if (car != nullptr) {
        // test:
        car->printCar();
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
        if (mainBoard.cleared()) {
            // succeed!
            ShowSuccessInfo();
            ifShowInstructions = false;
        }
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
    setKeyboard(clickedX, clickedY);

    Car* car = mainBoard.getCar(clickedX, clickedY);
    if (car != nullptr) {
        // test:
        car->printCar();
        if (car->direction == 1) { // in a row, move down.
            if (car->column + car->length < mainBoard.size &&
                mainBoard.data[car->row][car->column + car->length] == 0)
            {
                qDebug() << "before, column = " << car->column;
                car->column++;
                qDebug() << "column = " << car->column;
            }
        }
        else { // in a column, move right.
           if (car->row + car->length < mainBoard.size &&
               mainBoard.data[car->row + car->length][car->column] == 0) {
                qDebug() << "before, row = " << car->row;
               car->row++;
                qDebug() << "row = " << car->row;
           }
        }
        mainBoard.update();
        if (mainBoard.cleared()) {
            // succeed!
            ShowSuccessInfo();
            ifShowInstructions = false;
        }
        createGrid(); // draw new graph
    }

}
void MainWindow::ShowSuccessInfo() {
    // 创建 QMessageBox 实例
    QMessageBox msgBox;

    // 设置提示框的标题
    msgBox.setWindowTitle("提示");

    // 设置提示框的文本信息
    msgBox.setText("闯关成功！开始新游戏");

    // 设置提示框的图标（例如：QMessageBox::Information、QMessageBox::Warning等）
    msgBox.setIcon(QMessageBox::Information);

    // 添加按钮，例如：Ok、Cancel等
    msgBox.addButton(QMessageBox::Ok);

    // 显示提示框，并等待用户响应
    msgBox.exec();

    newGame();
}
void MainWindow::playBackgroundMusic() {
    QMediaPlaylist *playlist = new QMediaPlaylist();
    QString filePath = "F:/MyProject/Qt_projects/huarong-road-qt/LOL.mp3";
    QMediaContent mediaContent(QUrl::fromLocalFile(filePath));
    playlist->addMedia(mediaContent);
    // playlist->addMedia(QUrl::fromLocalFile("LOL.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer *music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->play();
}
