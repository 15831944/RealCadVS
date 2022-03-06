#ifndef SHADE_DIALOG_H
#define SHADE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Shade_Dialog;
}

class Shade_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Shade_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Shade_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Shade_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Shade_Dialog *ui;
};

#endif // SHADE_DIALOG_H
