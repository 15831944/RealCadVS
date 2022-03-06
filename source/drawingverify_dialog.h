#ifndef DRAWINGVERIFY_DIALOG_H
#define DRAWINGVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DrawingVerify_Dialog;
}

class DrawingVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DrawingVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DrawingVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DrawingVerify_Dialog();

signals:

public slots:
    void on__104_clicked(bool checked);
    void on_accept();


private:
    Ui::DrawingVerify_Dialog *ui;
};

#endif // DRAWINGVERIFY_DIALOG_H
