#ifndef STRETCH_DIALOG_H
#define STRETCH_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Stretch_Dialog;
}

class Stretch_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Stretch_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Stretch_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Stretch_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__105_clicked(bool checked);
    void on__106_clicked(bool checked);
    void on_accept();

private:
    Ui::Stretch_Dialog *ui;
};

#endif // STRETCH_DIALOG_H
