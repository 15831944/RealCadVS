#ifndef COPYALONG_DIALOG_H
#define COPYALONG_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CopyAlong_Dialog;
}

class CopyAlong_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CopyAlong_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CopyAlong_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CopyAlong_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on_accept();

private:
    Ui::CopyAlong_Dialog *ui;
};

#endif // COPYALONG_DIALOG_H
