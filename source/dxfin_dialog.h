#ifndef DXFIN_DIALOG_H
#define DXFIN_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DXFIn_Dialog;
}

class DXFIn_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DXFIn_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DXFIn_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DXFIn_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::DXFIn_Dialog *ui;
};

#endif // DXFIN_DIALOG_H
