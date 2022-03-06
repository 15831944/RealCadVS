#ifndef ONEVIEWCHANGE_DIALOG_H
#define ONEVIEWCHANGE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class OneViewChange_Dialog;
}

class OneViewChange_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit OneViewChange_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  OneViewChange_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~OneViewChange_Dialog();

signals:

public slots:
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on__111_clicked(bool checked);
    void on__114_clicked(bool checked);
    void on_accept();

private:
    Ui::OneViewChange_Dialog *ui;
};

#endif // ONEVIEWCHANGE_DIALOG_H
