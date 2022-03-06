#ifndef PATCHVERIFY_DIALOG_H
#define PATCHVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PatchVerify_Dialog;
}

class PatchVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PatchVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PatchVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PatchVerify_Dialog();

signals:

public slots:
    void on__109(bool checked); // change surface properties button
    void on__111_clicked(bool checked); // Compare button
    void on__112_clicked(bool checked); // Triangulate button

    void on__1000_clicked(bool checked); // colour select button
    void on__1001_clicked(bool checked); // layer select button
    void on__1002_clicked(bool checked); // style select button
    void on__1003_clicked(bool checked); // weight select button

    void on_accept();

private:
    Ui::PatchVerify_Dialog *ui;
};

#endif // PATCHVERIFY_DIALOG_H
