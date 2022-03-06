#ifndef DELETEVIEW_DIALOG_H
#define DELETEVIEW_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class DeleteView_Dialog;
}

class DeleteView_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DeleteView_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DeleteView_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DeleteView_Dialog();

signals:

public slots:
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::DeleteView_Dialog *ui;
};

#endif // DELETEVIEW_DIALOG_H
