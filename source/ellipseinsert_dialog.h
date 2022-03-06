#ifndef ELLIPSEINSERT_DIALOG_H
#define ELLIPSEINSERT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class EllipseInsert_Dialog;
}

class EllipseInsert_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit EllipseInsert_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE EllipseInsert_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~EllipseInsert_Dialog();

signals:
    
public slots:
    void on_accept();

private:
    Ui::EllipseInsert_Dialog *ui;
};

#endif // ELLIPSEINSERT_DIALOG_H
