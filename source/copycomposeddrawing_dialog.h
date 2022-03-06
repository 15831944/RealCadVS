#ifndef COPYCOMPOSEDDRAWING_DIALOG_H
#define COPYCOMPOSEDDRAWING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CopyComposedDrawing_Dialog;
}

class CopyComposedDrawing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CopyComposedDrawing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  CopyComposedDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CopyComposedDrawing_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::CopyComposedDrawing_Dialog *ui;
};

#endif // COPYCOMPOSEDDRAWING_DIALOG_H
