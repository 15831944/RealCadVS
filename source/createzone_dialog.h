#ifndef CREATEZONE_DIALOG_H
#define CREATEZONE_DIALOG_H


#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class CreateZone_Dialog;
}

class CreateZone_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CreateZone_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CreateZone_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CreateZone_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::CreateZone_Dialog *ui;
};

#endif // CREATEZONE_DIALOG_H
