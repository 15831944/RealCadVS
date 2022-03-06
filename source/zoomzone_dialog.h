#ifndef ZOOMZONE_DIALOG_H
#define ZOOMZONE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class ZoomZone_Dialog;
}

class ZoomZone_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ZoomZone_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE ZoomZone_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ZoomZone_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::ZoomZone_Dialog *ui;
};

#endif // ZOOMZONE_DIALOG_H
