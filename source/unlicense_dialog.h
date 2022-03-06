#ifndef UNLICENSE_DIALOG_H
#define UNLICENSE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Unlicense_Dialog;
}

class Unlicense_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Unlicense_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Unlicense_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Unlicense_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);

    void on_accept();
private:
    Ui::Unlicense_Dialog *ui;
};

#endif // UNLICENSE_DIALOG_H
