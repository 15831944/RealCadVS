#ifndef PATCHMESH_DIALOG_H
#define PATCHMESH_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class PatchMesh_Dialog;
}

class PatchMesh_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PatchMesh_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PatchMesh_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PatchMesh_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::PatchMesh_Dialog *ui;
};

#endif // PATCHMESH_DIALOG_H
