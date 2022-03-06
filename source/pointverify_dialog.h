#ifndef POINTVERIFY_DIALOG_H
#define POINTVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PointVerify_Dialog;
}

class PointVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PointVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PointVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PointVerify_Dialog();

signals:

public slots:
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);

    void on_accept();

private:
    Ui::PointVerify_Dialog *ui;
};

#endif // POINTVERIFY_DIALOG_H
