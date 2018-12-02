#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include <memory>
#include <vector>
#include <QByteArray>
#include <fstream>

class GameBoardModel
{
public:
    GameBoardModel(const int, const int);
    int row() const;
    void setRow(const int);
    int column() const;
    void setColumn(const int);
    bool cell(const int, const int) const;
    void setCell(const int, const int, const bool);
    void init(const int, const int);
    void load(std::ifstream&);
    void saveTo(std::ofstream&);
    void clear();
    void step();
private:
    bool check(const bool, const int);
    int index(const int, const int) const;
    int _row;
    int _column;
    int _generation;
    std::vector<bool> _board;
};

#endif // GAMEBOARDMODEL_H
