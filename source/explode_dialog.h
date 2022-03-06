#ifndef EXPLODE_DIALOG_H
#define EXPLODE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Explode_Dialog;
}

class Explode_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Explode_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Explode_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Explode_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::Explode_Dialog *ui;
};

#endif // EXPLODE_DIALOG_H
