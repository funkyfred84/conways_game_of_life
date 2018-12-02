#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QPaintEvent>
#include "gameboardmodel.h"
#include <memory>
#include <QPainter>
#include <QMouseEvent>
#include <QByteArray>
#include <QFile>
#include <fstream>

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(const int row = 10, const int column = 10, const int speed = 50, QWidget *parent = nullptr);
    void init(const int, const int, const int);
    int columns() const;
    int rows() const;
    bool load(std::ifstream&);
    void clear();
    void saveTo(std::ofstream&);
protected:
    virtual void paintEvent(QPaintEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
signals:

public slots:
    void step();

private:
    int _speed;
    void drawBoard(QPainter&);
    void drawModel(QPainter&);
    std::unique_ptr<GameBoardModel> model;

};

#endif // GAMEBOARD_H
