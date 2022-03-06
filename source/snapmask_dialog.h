#ifndef SNAPMASK_DIALOG_H
#define SNAPMASK_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class SnapMask_Dialog;
}

class SnapMask_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SnapMask_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SnapMask_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SnapMask_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on_accept();

private:
    Ui::SnapMask_Dialog *ui;
};

#endif // SNAPMASK_DIALOG_H
