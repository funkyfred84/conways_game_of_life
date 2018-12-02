#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "newgamedialog.h"
#include <QPaintEvent>
#include "gameboard.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:


private slots:
    void on_actionnew_triggered();

    void on_actionStart_triggered();

    void on_actionstop_triggered();

    void on_actionstep_triggered();

    void on_actionload_triggered();
    
    void on_actionsave_triggered();

private:

    Ui::MainWindow *ui;
    NewGameDialog* newGameDialog;
    GameBoard* gameboard;
    QTimer* timer;
};

#endif // MAINWINDOW_H
