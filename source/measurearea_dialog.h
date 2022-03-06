#ifndef MEASUREAREA_DIALOG_H
#define MEASUREAREA_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class MeasureArea_Dialog;
}

class MeasureArea_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit MeasureArea_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE MeasureArea_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~MeasureArea_Dialog();

signals:

public slots:
    //void on_accept();

private:
    Ui::MeasureArea_Dialog *ui;
};

#endif // MEASUREAREA_DIALOG_H
