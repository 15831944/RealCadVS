#ifndef SELECTWORKPLANE_DIALOG_H
#define SELECTWORKPLANE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class SelectWorkplane_Dialog;
}

class QListWidgetItem;

class SelectWorkplane_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectWorkplane_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SelectWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectWorkplane_Dialog();

signals:
    
public slots:
    void listDoubleClicked(QListWidgetItem *);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on_accept();
    
private:
    Ui::SelectWorkplane_Dialog *ui;
};

#endif // SELECTWORKPLANE_DIALOG_H
