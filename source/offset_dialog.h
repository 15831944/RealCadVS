#ifndef OFFSET_DIALOG_H
#define OFFSET_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Offset_Dialog;
}


class Offset_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Offset_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Offset_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Offset_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on_accept();

private:
    Ui::Offset_Dialog *ui;
};

#endif // OFFSET_DIALOG_H
