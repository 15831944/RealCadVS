#ifndef ROTATE_DIALOG_H
#define ROTATE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Rotate_Dialog;
}

class Rotate_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Rotate_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Rotate_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Rotate_Dialog();

signals:

public slots:
    void on__99_clicked(bool checked);
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::Rotate_Dialog *ui;
};

#endif // ROTATE_DIALOG_H
