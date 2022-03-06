#ifndef FINDINFILES_DIALOG_H
#define FINDINFILES_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class FindInFiles_Dialog;
}

class FindInFiles_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit FindInFiles_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  FindInFiles_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~FindInFiles_Dialog();

signals:

public slots:
    void on__101_clicked(bool checked);// browse path button
    void on__103_clicked(bool checked);// search text entities button
    void on__104_clicked(bool checked);// match case button
    void on__105_clicked(bool checked);// search attributes button
    void on__107_clicked(bool checked);// find button
    void on__108_clicked(bool checked);// open button

private:
    Ui::FindInFiles_Dialog *ui;
};

#endif // FINDINFILES_DIALOG_H
