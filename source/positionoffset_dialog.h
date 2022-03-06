#ifndef POSITIONOFFSET_DIALOG_H
#define POSITIONOFFSET_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class PositionOffset_Dialog;
}

class PositionOffset_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PositionOffset_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PositionOffset_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PositionOffset_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::PositionOffset_Dialog *ui;
};

#endif // POSITIONOFFSET_DIALOG_H
