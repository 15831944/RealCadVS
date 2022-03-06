#ifndef LEADERVERIFY_DIALOG_H
#define LEADERVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class LeaderVerify_Dialog;
}

class LeaderVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LeaderVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  LeaderVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LeaderVerify_Dialog();

signals:

public slots:
    void on__110_clicked(bool checked);// bold
    void on__111_clicked(bool checked);// underline
    void on__113_clicked(bool checked);// italic
    void on__114_clicked(bool checked);// boxed
    void on__112_clicked(bool checked);// outline
    void on__115_clicked(bool checked);// halo
    void on__121_clicked(bool checked);// scale text
    void on__119_clicked(bool checked);// specify terminator weight
    void on__2000_clicked(bool checked);// change button for testing

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on__1004_clicked(bool checked);
    void on__1005_clicked(bool checked);

    void on_accept();

private:
    Ui::LeaderVerify_Dialog *ui;
};

#endif // LEADERVERIFY_DIALOG_H
