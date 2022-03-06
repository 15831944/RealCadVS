#ifndef DELETEZONE_DIALOG_H
#define DELETEZONE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DeleteZone_Dialog;
}

class DeleteZone_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DeleteZone_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DeleteZone_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DeleteZone_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::DeleteZone_Dialog *ui;
};

#endif // DELETEZONE_DIALOG_H
