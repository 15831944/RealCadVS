#ifndef SELECTVIEW_DIALOG_H
#define SELECTVIEW_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class SelectView_Dialog;
}

class SelectView_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectView_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SelectView_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectView_Dialog();

signals:

public slots:
    void on__100_currentRowChanged(int currentRow);
    void on__101_currentRowChanged(int currentRow);
    void on_accept();

private:
    Ui::SelectView_Dialog *ui;
};

#endif // SELECTVIEW_DIALOG_H
