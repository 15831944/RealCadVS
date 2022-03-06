#ifndef CHAMFER_DIALOG_H
#define CHAMFER_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Chamfer_Dialog;
}

class Chamfer_Dialog :  public RCDialog
{
    Q_OBJECT
public:
    explicit Chamfer_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Chamfer_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Chamfer_Dialog();

signals:
    
public slots:
    void on__1102_actionTriggered(int action);
    void on__1103_actionTriggered(int action);
    void on__1104_actionTriggered(int action);
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::Chamfer_Dialog *ui;
};

#endif // CHAMFER_DIALOG_H
