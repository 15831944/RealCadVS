#ifndef SELECTDRAWING_DIALOG_H
#define SELECTDRAWING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SelectDrawing_Dialog;
}

class SelectDrawing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectDrawing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  SelectDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectDrawing_Dialog();

signals:

public slots:
    void on__100_itemPressed(QListWidgetItem * item);
    void on__100_itemDoubleClicked(QListWidgetItem * item);
    void on__100_currentRowChanged(int currentRow);
    void on__101_currentRowChanged(int currentRow);
    void on_accept();

private:
    Ui::SelectDrawing_Dialog *ui;
};

#endif // SELECTDRAWING_DIALOG_H
