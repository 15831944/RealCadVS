#ifndef LINEVERIFY_DIALOG_H
#define LINEVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class LineVerify_Dialog;
}

class LineVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LineVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE LineVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LineVerify_Dialog();

signals:

public slots:
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on_accept();

private:
    Ui::LineVerify_Dialog *ui;
};

#endif // LINEVERIFY_DIALOG_H
