#ifndef POSITIONALONG_DIALOG_H
#define POSITIONALONG_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class PositionAlong_Dialog;
}

class PositionAlong_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PositionAlong_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PositionAlong_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PositionAlong_Dialog();

signals:

public slots:
    void on__200_clicked(bool checked);
    void on__201_clicked(bool checked);
    void on__202_clicked(bool checked);
    void on_accept();

private:
    Ui::PositionAlong_Dialog *ui;
};

#endif // POSITIONALONG_DIALOG_H
