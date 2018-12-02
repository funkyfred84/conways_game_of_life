#include "gameboardmodel.h"
#include <QDebug>
#include <stdexcept>

GameBoardModel::GameBoardModel(const int row , const int column) : _row(row), _column(column), _board(std::vector<bool>(row * column, false)), _generation(0)
{

}

int GameBoardModel::row() const
{
    return _row;
}

void GameBoardModel::setRow(const int row)
{
    if (_row == row) return;
    if (row < 1) return;
    _row = row;
}

int GameBoardModel::column() const
{
    return _column;
}

void GameBoardModel::setColumn(const int column)
{
    if (_column == column) return;
    if (column < 1) return;
    _column = column;
}

bool GameBoardModel::cell(const int row, const int column) const
{
    return _board[index(row, column)];
}

void GameBoardModel::setCell(const int row, const int column, const bool value)
{
    _board[index(row, column)] = value;
}

void GameBoardModel::init(const int rows, const int columns)
{
    setRow(rows);
    setColumn(columns);
    _generation = 0;
    clear();
    _board.resize(_row * _column, false);

}

void GameBoardModel::load(std::ifstream &iff)
{

    iff.read(reinterpret_cast<char*>(&_row), sizeof (int));
    iff.read(reinterpret_cast<char*>(&_column), sizeof (int));
    std::vector<uint8_t> data(_row * _column / (8 * sizeof (uint8_t)) + 1, 0x00);
    iff.read(reinterpret_cast<char*>(data.data()), data.size());
    for (int i = 0; i < _row * _column; i++) {
        _board[i] = (data[i/8] & (0x01 << (i % 8))) != 0x00;
    }
}

void GameBoardModel::saveTo(std::ofstream &off)
{
    off.write(reinterpret_cast<char*>(&_row), sizeof (int));
    off.write(reinterpret_cast<char*>(&_column), sizeof (int));

    std::vector<uint8_t> data(_row * _column / (8 * sizeof (uint8_t)) + 1, 0x00);
    for (int i = 0; i < _row * _column; i++) {
        data[i/8] = data[i/8] | (_board[i] ? (0x01 << (i % 8)) : 0x00);
    }
    for (auto b : data) {
        off << b;
    }

}

void GameBoardModel::clear()
{
    for (int i = 0; i < _board.size(); i++) {
        _board[i] = false;
    }

}

void GameBoardModel::step()
{
    _generation++;
    std::vector<bool> copy(_board);
    for (int row = 1; row < _row - 1; row++) {
        for (int column = 1; column < _column - 1; column++) {
            int countAlive = 0;
            for (int i = 0; i < 9; i++) {
                int dx = (i % 3) - 1;
                int dy = (i / 3) - 1;
                if (dx != 0 || dy != 0) {
                    if (cell(dy + row, dx + column)) {
                        countAlive++;
                    }
                }
            }
            qDebug() << "cell (" << row << "/" << column << ") active neighbour = " << countAlive;
            copy[index(row, column)] = check(cell(row, column), countAlive);
        }
    }
    int countAlive = 0;
    countAlive += cell(0, 1) ? 1 : 0;
    countAlive += cell(1, 0) ? 1 : 0;
    countAlive += cell(1, 1) ? 1 : 0;
    copy[index(0, 0)] = check(cell(0, 0), countAlive);

    countAlive = 0;
    countAlive += cell(0, _column - 2) ? 1 : 0;
    countAlive += cell(1, _column - 1) ? 1 : 0;
    countAlive += cell(1, _column - 2) ? 1 : 0;
    copy[index(0, _column - 1)] = check(cell(0, _column - 1), countAlive);

    countAlive = 0;
    countAlive += cell(_row - 1, _column - 2) ? 1 : 0;
    countAlive += cell(_row - 2, _column - 1) ? 1 : 0;
    countAlive += cell(_row - 2, _column - 2) ? 1 : 0;
    copy[index(_row - 1, _column - 1)] = check(cell(_row - 1, _column - 1), countAlive);

    countAlive = 0;
    countAlive += cell(_row - 1, 1) ? 1 : 0;
    countAlive += cell(_row - 2, 0) ? 1 : 0;
    countAlive += cell(_row - 2, 1) ? 1 : 0;
    copy[index(_row - 1, 0)] = check(cell(_row - 1, 0), countAlive);

    int countAliveLeft;
    int countAliveRight;
    for (int i = 1; i < _row - 1; i++) {
        countAliveLeft = 0;
        countAliveLeft += cell(i - 1, 0) ? 1 : 0;
        countAliveLeft += cell(i, 1) ? 1 : 0;
        countAliveLeft += cell(i - 1, 1) ? 1 : 0;
        countAliveLeft += cell(i + 1, 0) ? 1 : 0;
        countAliveLeft += cell(i + 1, 1) ? 1 : 0;
        copy[index(i, 0)] = check(cell(i, 0), countAliveLeft);

        countAliveRight = 0;
        countAliveRight += cell(i - 1, _column - 1) ? 1 : 0;
        countAliveRight += cell(i - 1, _column - 2) ? 1 : 0;
        countAliveRight += cell(i, _column - 2) ? 1 : 0;
        countAliveRight += cell(i + 1, _column - 1) ? 1 : 0;
        countAliveRight += cell(i + 1, _column - 2) ? 1 : 0;
        copy[index(i, _column - 1)] = check(cell(i, _column - 1), countAliveRight);
    }

    int countAliveUp;
    int countAliveDown;
    for (int i = 1; i < _column - 1; i++) {
        countAliveUp = 0;
        countAliveUp += cell(0, i - 1) ? 1 : 0;
        countAliveUp += cell(0, i + 1) ? 1 : 0;
        countAliveUp += cell(1, i - 1) ? 1 : 0;
        countAliveUp += cell(1, i) ? 1 : 0;
        countAliveUp += cell(1, i + 1) ? 1 : 0;
        copy[index(0, i)] = check(cell(0, i), countAliveUp);

        countAliveDown = 0;
        countAliveDown += cell(_row - 1, i - 1) ? 1 : 0;
        countAliveDown += cell(_row - 1, i + 1) ? 1 : 0;
        countAliveDown += cell(_row - 2, i - 1) ? 1 : 0;
        countAliveDown += cell(_row - 2, i) ? 1 : 0;
        countAliveDown += cell(_row - 2, i + 1) ? 1 : 0;
        copy[index(_row - 1, i)] = check(cell(_row - 1, i), countAliveDown);
    }

    _board.swap(copy);
    qDebug() << "finished";
}

bool GameBoardModel::check(const bool cellValue, const int aliveNeighbours)
{
    if (cellValue) {
        return 2 <= aliveNeighbours && 3 >= aliveNeighbours;
    } else {
        return 3 == aliveNeighbours;
    }
}

int GameBoardModel::index(const int row, const int column) const
{
    if (0 > row || _row <= row) {
        throw std::out_of_range("row out of range");
    }
    if (0 > column || _column <= column) {
        throw std::out_of_range("column out of range");
    }
    auto ret = column + row * _column;
    return ret;
}

