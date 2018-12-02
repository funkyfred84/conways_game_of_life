#include "gameboard.h"
#include <QPainter>
#include <QDebug>
#include <QByteArray>
#include <QDataStream>

GameBoard::GameBoard(const int row, const int column, const int speed, QWidget *parent) : QWidget(parent), _speed(speed), model(std::make_unique<GameBoardModel>(row, column))
{
}

void GameBoard::init(const int rows, const int columns, const int speed)
{
    _speed = speed;
    model->init(rows, columns);
}

int GameBoard::columns() const
{
    return model->column();
}

int GameBoard::rows() const
{
    return model->row();
}

bool GameBoard::load(std::ifstream &iff)
{
    iff.read(reinterpret_cast<char*>(&_speed), sizeof (int));
    model->load(iff);
    return true;
}

void GameBoard::clear()
{
    model->clear();
}

void GameBoard::saveTo(std::ofstream &off)
{
    off.write(reinterpret_cast<char*>(&_speed), sizeof(int));
    model->saveTo(off);
}

void GameBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1));
    drawBoard(painter);
    painter.setBrush(QBrush(Qt::black));
    drawModel(painter);
}

void GameBoard::mouseReleaseEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    int r = model->row();
    int c = model->column();
    int w = width();
    int h = height();

    double dx = static_cast<double>(w) / static_cast<double>(c);
    int xCell = static_cast<int>(static_cast<double>(x) / dx);

    double dy = static_cast<double>(h) / static_cast<double>(r);
    int yCell = static_cast<int>(static_cast<double>(y) / dy);
    model->setCell(yCell, xCell, !(model->cell(yCell, xCell)));
    update();
}

void GameBoard::mouseMoveEvent(QMouseEvent *e)
{
    int x = e->x();
    int y = e->y();
    int r = model->row();
    int c = model->column();
    int w = width();
    int h = height();

    double dx = static_cast<double>(w) / static_cast<double>(c);
    int xCell = static_cast<int>(static_cast<double>(x) / dx);

    double dy = static_cast<double>(h) / static_cast<double>(r);
    int yCell = static_cast<int>(static_cast<double>(y) / dy);
    qDebug() << "Mouse (" << x << "/" << y << ") pressed Cell(" << yCell << "/" << xCell << ")";

}

void GameBoard::step()
{
    model->step();
    update();
}

void GameBoard::drawBoard(QPainter &painter)
{
    int w = width();
    int h = height();
    int r = model->row();
    int c = model->column();
    double dx = static_cast<double>(w) / static_cast<double>(c);
    double dy = static_cast<double>(h) / static_cast<double>(r);

    for (int i = 1; i < r; i++) {
        painter.drawLine(0, static_cast<int>(i * dy), w, static_cast<int>(i * dy));
    }
    for (int i = 1; i < c; i++) {
        painter.drawLine(static_cast<int>(i * dx), 0, static_cast<int>(i * dx), h);
    }
}

void GameBoard::drawModel(QPainter &painter)
{
    int w = width();
    int h = height();
    int r = model->row();
    int c = model->column();
    double dx = static_cast<double>(w) / static_cast<double>(c);
    double dy = static_cast<double>(h) / static_cast<double>(r);


    for (int i = 0; i < model->row(); i++) {
        for (int j = 0; j < model->column(); j++) {
            if (model->cell(i, j)) {
                painter.drawRect(j * dx, i * dy, dx, dy);
            }
        }
    }
}
