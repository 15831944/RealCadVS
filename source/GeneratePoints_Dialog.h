#ifndef GENERATEPOINTS_DIALOG_H
#define GENERATEPOINTS_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class GeneratePoints_Dialog;
}

//class GeneratePoints_Dialog : public QDialog, public RCDialog
class GeneratePoints_Dialog : public RCDialog
{
    Q_OBJECT

    //friend long dialogcb(HWND,UINT,WPARAM,LPARAM);
public:
    explicit GeneratePoints_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f = 0);
	Q_INVOKABLE GeneratePoints_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~GeneratePoints_Dialog();

private slots:
    // these two functions are not used anymore
    // just here as an example of how to use the scroll bars
    void on__1104_actionTriggered(int action);
    void on__1105_actionTriggered(int action);
    // these are the change functions for a Qt SpinBox control
    // not currently by the code
    void on__104_valueChanged(int value);
    void on__105_valueChanged(double value);
    //
    void on__102_toggled(bool checked);
    void on__103_toggled(bool checked);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__1_clicked(bool checked);
    void on__2_clicked(bool checked);
    void on_accept();

private:
    Ui::GeneratePoints_Dialog *ui;
};

#endif // GENERATEPOINTS_DIALOG_H
