#ifndef MODIFYPARAMETRICFIGURE_DIALOG_H
#define MODIFYPARAMETRICFIGURE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class ModifyParametricFigure_Dialog;
}

class ModifyParametricFigure_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ModifyParametricFigure_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  ModifyParametricFigure_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ModifyParametricFigure_Dialog();

signals:

public slots:

private slots:
    void on__1_clicked();

private:
    Ui::ModifyParametricFigure_Dialog *ui;
};

#endif // MODIFYPARAMETRICFIGURE_DIALOG_H
