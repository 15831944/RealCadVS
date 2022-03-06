#include "rconecoordinateentry.h"
#include "ui_rconecoordinateentry.h"

RCOneCoordinateEntry::RCOneCoordinateEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RCOneCoordinateEntry)
{
    ui->setupUi(this);
}

QLabel * RCOneCoordinateEntry::getPrompt()
{
    return ui->Label;
}

QLabel * RCOneCoordinateEntry::getIcon()
{
    return ui->Icon;
}

QLineEdit * RCOneCoordinateEntry::getEdit()
{
    return ui->Edit;
}
