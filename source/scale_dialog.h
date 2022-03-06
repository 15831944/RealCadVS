#ifndef SCALE_DIALOG_H
#define SCALE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Scale_Dialog;
}

class Scale_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Scale_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Scale_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Scale_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on_accept();

private:
    Ui::Scale_Dialog *ui;
};

#endif // SCALE_DIALOG_H
