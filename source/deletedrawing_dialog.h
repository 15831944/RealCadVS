#ifndef DELETEDRAWING_DIALOG_H
#define DELETEDRAWING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DeleteDrawing_Dialog;
}

class DeleteDrawing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DeleteDrawing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DeleteDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DeleteDrawing_Dialog();

signals:

public slots:
    void on__101_clicked(bool checked);// delete button
    void on__100_listItemDoubleClicked(QListWidgetItem * item);

private:
    Ui::DeleteDrawing_Dialog *ui;
};

#endif // DELETEDRAWING_DIALOG_H
