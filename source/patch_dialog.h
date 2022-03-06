#ifndef PATCH_DIALOG_H
#define PATCH_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Patch_Dialog;
}

class Patch_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Patch_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Patch_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Patch_Dialog();

    void drawSample();
signals:

public slots:
    void on__100_valueChanged(int v); // colour
    void on__101_valueChanged(int v); // style
    void on__102_valueChanged(int v); // layer
    void on__103_valueChanged(double v); // weight

    void on__104_valueChanged(double v); // x degree
    void on__105_valueChanged(double v); // y degree
    void on__106_valueChanged(int v); // X mesh
    void on__107_valueChanged(int v); // Y mesh

    void on__108_currentIndexChanged(int index); // edge mode combo box

    void on__110_clicked(bool checked); // surface properties
    void on__111_clicked(bool checked); // compare
    void on__112_clicked(bool checked); // triagulate
    void on__113_clicked(bool checked); // clone all (106->113)
    void on__114_clicked(bool checked); // clone (105->114)

    void on__1000_clicked(bool checked);// colour select button
    void on__1001_clicked(bool checked);// layer select button
    void on__1002_clicked(bool checked);// style select button
    void on__1003_clicked(bool checked);// weight select button
    void on_accept();

private:
    Ui::Patch_Dialog *ui;
};

#endif // PATCH_DIALOG_H
