#ifndef COPYBITMAP_DIALOG_H
#define COPYBITMAP_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CopyBitmap_Dialog;
}

class CopyBitmap_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CopyBitmap_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CopyBitmap_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CopyBitmap_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::CopyBitmap_Dialog *ui;
};

#endif // COPYBITMAP_DIALOG_H
