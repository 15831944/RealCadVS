#ifndef CONVERTTOCURVE_DIALOG_H
#define CONVERTTOCURVE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class ConvertToCurve_Dialog;
}

class ConvertToCurve_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ConvertToCurve_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE ConvertToCurve_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ConvertToCurve_Dialog();
    
signals:
    
public slots:
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::ConvertToCurve_Dialog *ui;
};

#endif // CONVERTTOCURVE_DIALOG_H
