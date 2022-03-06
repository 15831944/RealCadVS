#ifndef RESETVIEW_DIALOG_H
#define RESETVIEW_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class ResetView_Dialog;
}

class ResetView_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ResetView_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE ResetView_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ResetView_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::ResetView_Dialog *ui;
};

#endif // RESETVIEW_DIALOG_H
