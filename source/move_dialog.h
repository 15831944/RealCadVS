#ifndef MOVE_DIALOG_H
#define MOVE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Move_Dialog;
}

class Move_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Move_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Move_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Move_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::Move_Dialog *ui;
};

#endif // MOVE_DIALOG_H
