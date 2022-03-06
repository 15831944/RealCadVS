#ifndef DELETEWORKPLANE_DIALOG_H
#define DELETEWORKPLANE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class DeleteWorkplane_Dialog;
}

class DeleteWorkplane_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DeleteWorkplane_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DeleteWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DeleteWorkplane_Dialog();

signals:
    
public slots:
    void on_accept();

private:
    Ui::DeleteWorkplane_Dialog *ui;
};

#endif // DELETEWORKPLANE_DIALOG_H
