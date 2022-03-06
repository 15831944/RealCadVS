#ifndef REFERENCE_DIALOG_H
#define REFERENCE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Reference_Dialog;
}


class Reference_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Reference_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Reference_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Reference_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Reference_Dialog *ui;
};

#endif // REFERENCE_DIALOG_H
