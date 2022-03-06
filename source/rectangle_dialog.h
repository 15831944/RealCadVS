#ifndef RECTANGLE_DIALOG_H
#define RECTANGLE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Rectangle_Dialog;
}

//class Rectangle_Dialog : public QDialog, public RCDialog
class Rectangle_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Rectangle_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Rectangle_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Rectangle_Dialog();

    //bool eventFilter(QObject *obj, QEvent *event);

signals:
    
public slots:
    void on__1103_actionTriggered(int action);
    void on__1104_actionTriggered(int action);
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__105_clicked(bool checked);
    void on__106_clicked(bool checked);
    void on_accept();

private:
    Ui::Rectangle_Dialog *ui;
};

#endif // RECTANGLE_DIALOG_H
