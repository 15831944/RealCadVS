#ifndef ELLIPSEVERIFY_DIALOG_H
#define ELLIPSEVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class EllipseVerify_Dialog;
}

class EllipseVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit EllipseVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE EllipseVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~EllipseVerify_Dialog();

signals:

public slots:
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on_accept();

private:
    Ui::EllipseVerify_Dialog *ui;
};

#endif // ELLIPSEVERIFY_DIALOG_H
