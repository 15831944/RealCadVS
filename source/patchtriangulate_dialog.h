#ifndef PATCHTRIANGULATE_DIALOG_H
#define PATCHTRIANGULATE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PatchTriangulate_Dialog;
}

class PatchTriangulate_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PatchTriangulate_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PatchTriangulate_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PatchTriangulate_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::PatchTriangulate_Dialog *ui;
};

#endif // PATCHTRIANGULATE_DIALOG_H
