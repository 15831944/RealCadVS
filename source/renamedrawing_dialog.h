#ifndef RENAMEDRAWING_DIALOG_H
#define RENAMEDRAWING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class RenameDrawing_Dialog;
}

class RenameDrawing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit RenameDrawing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  RenameDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~RenameDrawing_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::RenameDrawing_Dialog *ui;
};

#endif // RENAMEDRAWING_DIALOG_H
