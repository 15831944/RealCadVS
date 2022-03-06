#ifndef MEASUREANGLE_DIALOG_H
#define MEASUREANGLE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class MeasureAngle_Dialog;
}

class MeasureAngle_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit MeasureAngle_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE MeasureAngle_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~MeasureAngle_Dialog();

signals:

public slots:
    //void on_accept();

private:
    Ui::MeasureAngle_Dialog *ui;
};

#endif // MEASUREANGLE_DIALOG_H
