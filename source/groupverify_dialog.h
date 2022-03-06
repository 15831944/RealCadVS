#ifndef GROUPVERIFY_DIALOG_H
#define GROUPVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class GroupVerify_Dialog;
}

class GroupVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit GroupVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE GroupVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~GroupVerify_Dialog();

signals:

public slots:
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on_accept();

private:
    Ui::GroupVerify_Dialog *ui;
};

#endif // GROUPVERIFY_DIALOG_H
