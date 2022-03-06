#ifndef OFFSETD_DIALOG_H
#define OFFSETD_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Offsetd_Dialog;
}

class Offsetd_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Offsetd_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Offsetd_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Offsetd_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on_accept();

private:
    Ui::Offsetd_Dialog *ui;
};

#endif // OFFSETD_DIALOG_H
