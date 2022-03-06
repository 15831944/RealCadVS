#ifndef INFORMATION_DIALOG_H
#define INFORMATION_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"


namespace Ui {
class Information_Dialog;
}

class Information_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit Information_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Information_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Information_Dialog();

protected:
    bool eventFilter(QObject *obj, QEvent *e);
signals:

public slots:
    void on_accept();

private:
    Ui::Information_Dialog *ui;
};

#endif // INFORMATION_DIALOG_H
