#ifndef CREATEVIEW_DIALOG_H
#define CREATEVIEW_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class CreateView_Dialog;
}

class CreateView_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CreateView_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CreateView_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CreateView_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::CreateView_Dialog *ui;
};

#endif // CREATEVIEW_DIALOG_H
