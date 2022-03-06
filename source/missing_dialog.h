#ifndef MISSING_DIALOG_H
#define MISSING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Missing_Dialog;
}

class Missing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Missing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Missing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Missing_Dialog();

signals:

public slots:
    void on__101_clicked(bool checked); // delete
    void on__102_clicked(bool checked); // show as text file

private:
    Ui::Missing_Dialog *ui;
};

#endif // MISSING_DIALOG_H
