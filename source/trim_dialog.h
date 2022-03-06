#ifndef TRIM_DIALOG_H
#define TRIM_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Trim_Dialog;
}

class Trim_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Trim_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Trim_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Trim_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Trim_Dialog *ui;
};

#endif // TRIM_DIALOG_H
