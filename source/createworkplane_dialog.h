#ifndef CREATEWORKPLANE_DIALOG_H
#define CREATEWORKPLANE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class CreateWorkplane_Dialog;
}

class CreateWorkplane_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CreateWorkplane_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CreateWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CreateWorkplane_Dialog();

signals:
    
public slots:
    void on_accept();

private:
    Ui::CreateWorkplane_Dialog *ui;
};

#endif // CREATEWORKPLANE_DIALOG_H
