#ifndef SELECTUNLOADAPPLICATION_DIALOG_H
#define SELECTUNLOADAPPLICATION_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SelectUnloadApplication_Dialog;
}

class SelectUnloadApplication_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectUnloadApplication_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  SelectUnloadApplication_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectUnloadApplication_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::SelectUnloadApplication_Dialog *ui;
};

#endif // SELECTUNLOADAPPLICATION_DIALOG_H
