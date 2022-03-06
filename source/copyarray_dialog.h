#ifndef COPYARRAY_DIALOG_H
#define COPYARRAY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CopyArray_Dialog;
}

class CopyArray_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CopyArray_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CopyArray_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CopyArray_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::CopyArray_Dialog *ui;
};

#endif // COPYARRAY_DIALOG_H
