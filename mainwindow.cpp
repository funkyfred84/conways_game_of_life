#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QLine>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QIODevice>
#include <fstream>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    newGameDialog(new NewGameDialog(this)),
    gameboard(new GameBoard(10, 10, 50, this)),
    timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(gameboard);
    connect(timer, &QTimer::timeout, gameboard, &GameBoard::step);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_actionnew_triggered()
{
    qDebug() << "show new dialog";
    newGameDialog->init();
    newGameDialog->show();
    if(newGameDialog->exec() == QDialog::Accepted) {
        qDebug() << "dialog ok pressed";
        int columns = newGameDialog->columns();
        int rows = newGameDialog->rows();
        int speed = 100 + 20 * newGameDialog->speed();
        gameboard->init(rows, columns, speed);
        timer->setInterval(speed);
        gameboard->clear();
        update();
    }

}

void MainWindow::on_actionStart_triggered()
{
    timer->start();
}

void MainWindow::on_actionstop_triggered()
{
    timer->stop();
}


void MainWindow::on_actionstep_triggered()
{
    timer->singleShot(1000, gameboard, &GameBoard::step);
}

void MainWindow::on_actionload_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Binary blob");
    QFile file(fileName);
    if (!file.exists()) return;
    std::ifstream iff(fileName.toStdString().c_str(), std::ios::in | std::ios::binary);
    gameboard->load(iff);
    iff.close();
    update();
}

void MainWindow::on_actionsave_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, "Binary blob");
    QFile file(fileName);
    if (file.exists()) {
        file.remove();
    }
    std::ofstream off(fileName.toStdString().c_str(), std::ios::out | std::ios::binary);
    gameboard->saveTo(off);
    off.flush();
    off.close();


}
