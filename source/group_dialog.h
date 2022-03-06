#ifndef GROUP_DIALOG_H
#define GROUP_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Group_Dialog;
}

class Group_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Group_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Group_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Group_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Group_Dialog *ui;
};

#endif // GROUP_DIALOG_H
