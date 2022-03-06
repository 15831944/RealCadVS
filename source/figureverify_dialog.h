#ifndef FIGUREVERIFY_DIALOG_H
#define FIGUREVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class FigureVerify_Dialog;
}

class FigureVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit FigureVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  FigureVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~FigureVerify_Dialog();

signals:

public slots:   
    void on__113_clicked(bool checked);
    void on__114_clicked(bool checked);// Modify Parametric Figure
    void on__115_clicked(bool checked);
    void on__116_clicked(bool checked);
    void on__117_clicked(bool checked);
    void on__119_clicked(bool checked);

    void on__1000_clicked(bool checked);// colour select button
    void on__1001_clicked(bool checked);// layer select button
    void on__1002_clicked(bool checked);// style select button
    void on__1003_clicked(bool checked);// weight select button
    void on_accept();

private:
    Ui::FigureVerify_Dialog *ui;
};

#endif // FIGUREVERIFY_DIALOG_H
