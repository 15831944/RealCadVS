#ifndef STANDARD_DBL_DIALOG_H
#define STANDARD_DBL_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Standard_Dbl_Dialog;
}

class Standard_Dbl_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit Standard_Dbl_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Standard_Dbl_Dialog(HANDLE parent, HANDLE rcparent, int f);
     ~Standard_Dbl_Dialog();

signals:
    
public slots:
    void on_accept( );

private:
    Ui::Standard_Dbl_Dialog *ui;
};

#endif // STANDARD_DBL_DIALOG_H
