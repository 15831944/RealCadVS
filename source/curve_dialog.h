#ifndef CURVE_DIALOG_H
#define CURVE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Curve_Dialog;
}

class Curve_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit Curve_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Curve_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Curve_Dialog();
    
signals:
    
public slots:
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::Curve_Dialog *ui;
};

#endif // CURVE_DIALOG_H
