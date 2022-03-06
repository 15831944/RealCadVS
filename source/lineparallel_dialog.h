#ifndef LINEPARALLEL_DIALOG_H
#define LINEPARALLEL_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class LineParallel_Dialog;
}

//class LineParallel_Dialog : public QDialog, public RCDialog
class LineParallel_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LineParallel_Dialog(QWidget *parent=0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE LineParallel_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LineParallel_Dialog();

signals:
    
public slots:
    void on__1103_actionTriggered(int action);
    void on__1104_actionTriggered(int action);
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__105_clicked(bool checked);
    void on__106_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on_accept();
private:
    Ui::LineParallel_Dialog *ui;
};

#endif // LINEPARALLEL_DIALOG_H
