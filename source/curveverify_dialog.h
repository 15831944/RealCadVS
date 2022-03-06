#ifndef CURVEVERIFY_DIALOG_H
#define CURVEVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CurveVerify_Dialog;
}

class CurveVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CurveVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CurveVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CurveVerify_Dialog();

signals:

public slots:
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);

    void on_accept();

private:
    Ui::CurveVerify_Dialog *ui;
};

#endif // CURVEVERIFY_DIALOG_H
