#include "newgamedialog.h"
#include "ui_newgamedialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

int NewGameDialog::rows()
{
    return ui->rows->value();
}

void NewGameDialog::setRows(int value)
{
    if (ui->rows->value() == value) return;
    if (value <= ui->rows->maximum() && value >= ui->rows->minimum()) {
        ui->rows->setValue(value);
    }
}

int NewGameDialog::columns()
{
    return ui->columns->value();
}

void NewGameDialog::setColumns(int value)
{
    if (ui->columns->value() == value) return;
    if (value <= ui->columns->maximum() && value >= ui->columns->minimum()) {
        ui->columns->setValue(value);
    }
}

int NewGameDialog::speed()
{
    return ui->speed->value();
}

void NewGameDialog::setSpeed(int value)
{
    if (ui->speed->value() == value) return;
    if (value <= ui->speed->maximum() && value >= ui->speed->minimum()) {
        ui->speed->setValue(value);
    }
}

void NewGameDialog::init()
{
    ui->rows->setValue(10);
    ui->columns->setValue(10);
    ui->speed->setValue(0);
}
