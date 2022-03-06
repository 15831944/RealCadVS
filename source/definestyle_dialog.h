#ifndef DEFINESTYLE_DIALOG_H
#define DEFINESTYLE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class DefineStyle_Dialog;
}

class DefineStyle_Dialog : public RCDialog
{

    Q_OBJECT
public:
    explicit DefineStyle_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
    ~DefineStyle_Dialog();

    void draw_sample();

signals:
    
public slots:
    void on__1_clicked(bool checked);
    void on__2_clicked(bool checked);
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(double v);
    void on__104_valueChanged(double v);
    void on__106_clicked(bool checked);

private:
    Ui::DefineStyle_Dialog *ui;
};

#endif // DEFINESTYLE_DIALOG_H
