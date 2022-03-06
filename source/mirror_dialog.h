#ifndef MIRROR_DIALOG_H
#define MIRROR_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Mirror_Dialog;
}

class Mirror_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Mirror_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Mirror_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Mirror_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::Mirror_Dialog *ui;
};

#endif // MIRROR_DIALOG_H
