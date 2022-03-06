#ifndef ToolBars_Dialog_H
#define ToolBars_Dialog_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class ToolBars_Dialog;
}

class ToolBars_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ToolBars_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  ToolBars_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ToolBars_Dialog();

signals:

public slots:
    //void on_accept();
    void on__100_itemClicked(QListWidgetItem * item);
    void on__100_itemDoubleClicked(QListWidgetItem * item);
    void on__101_itemClicked(QListWidgetItem * item);
    void on__101_itemDoubleClicked(QListWidgetItem * item);
    void on__102_clicked();
    void on__103_clicked();
    void on__104_clicked();
    void on__105_clicked();

private:
    Ui::ToolBars_Dialog *ui;
};

#endif // ToolBars_Dialog_H
