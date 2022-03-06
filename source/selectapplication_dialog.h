#ifndef SELECTAPPLICATION_DIALOG_H
#define SELECTAPPLICATION_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SelectApplication_Dialog;
}

class SelectApplication_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectApplication_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  SelectApplication_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectApplication_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::SelectApplication_Dialog *ui;
};

#endif // SELECTAPPLICATION_DIALOG_H
