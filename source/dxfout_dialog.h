#ifndef DXFOUT_DIALOG_H
#define DXFOUT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DXFOut_Dialog;
}

class DXFOut_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DXFOut_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DXFOut_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DXFOut_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::DXFOut_Dialog *ui;
};

#endif // DXFOUT_DIALOG_H
