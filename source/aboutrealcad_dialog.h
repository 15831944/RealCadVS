#ifndef ABOUTREALCAD_DIALOG_H
#define ABOUTREALCAD_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class AboutRealCAD_Dialog;
}

class AboutRealCAD_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit AboutRealCAD_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  AboutRealCAD_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~AboutRealCAD_Dialog();

public slots:
    void on__104_clicked();
    void on__106_clicked();

    void on_accept();
private:
    Ui::AboutRealCAD_Dialog *ui;
};

#endif // ABOUTREALCAD_DIALOG_H
