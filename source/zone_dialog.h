#ifndef ZONE_DIALOG_H
#define ZONE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Zone_Dialog;
}

class Zone_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Zone_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Zone_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Zone_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Zone_Dialog *ui;
};

#endif // ZONE_DIALOG_H
