#ifndef LINEANGLE_DIALOG_H
#define LINEANGLE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class LineAngle_Dialog;
}

//class LineAngle_Dialog : public QDialog, public RCDialog
class LineAngle_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LineAngle_Dialog(QWidget *parent=0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE LineAngle_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LineAngle_Dialog();

signals:
    
public slots:
    void on__101_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on_accept();

private:
    Ui::LineAngle_Dialog *ui;
};

#endif // LINEANGLE_DIALOG_H
