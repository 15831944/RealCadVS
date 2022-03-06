#ifndef PROJECT_DIALOG_H
#define PROJECT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Project_Dialog;
}

class Project_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Project_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Project_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Project_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on_accept();

private:
    Ui::Project_Dialog *ui;
};

#endif // PROJECT_DIALOG_H
