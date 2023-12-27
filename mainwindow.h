#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QComboBox>
#include <QtMultimedia/QMediaPlayer>

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

    void keyPressEvent(QKeyEvent *event);
    void handKeyPress_CtrlA();
    void handKeyPress_CtrlW();
    void handKeyPress_CtrlD();
    void handKeyPress_CtrlS();
    void handKeyPress_A();
    void handKeyPress_W();
    void handKeyPress_D();
    void handKeyPress_S();

    bool ifShowInstructions;
    void setKeyboard(int x, int y) {
        keyBoardRow = x;
        keyBoardColumn = y;
    }


private slots:
    void newGame();
    void instructions();
private:
    void createGrid();
    void generateMenu();
    void generatePromptMsg();
    void ShowSuccessInfo();

    int keyBoardRow;
    int keyBoardColumn;

    Board mainBoard; // board to show

    QGraphicsScene *scene;
    QGraphicsView *view;
    QVector<QVector<QGraphicsRectItem*>> gridCells;
    QComboBox *difficultyComboBox; // Added ComboBox for difficulty selection

    QMediaPlayer *backgroundMusicPlayer;
};

#endif // MAINWINDOW_H
