#ifndef PATCHCORNERS_DIALOG_H
#define PATCHCORNERS_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PatchCorners_Dialog;
}

class PatchCorners_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PatchCorners_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PatchCorners_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PatchCorners_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::PatchCorners_Dialog *ui;
};

#endif // PATCHCORNERS_DIALOG_H
