#ifndef Standard_Int_Dialog_H
#define Standard_Int_Dialog_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Standard_Int_Dialog;
}

class Standard_Int_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit Standard_Int_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Standard_Int_Dialog(HANDLE parent, HANDLE rcparent, int f);
     ~Standard_Int_Dialog();

signals:
    
public slots:
    void on_accept( );

private:
    Ui::Standard_Int_Dialog *ui;
};

#endif // Standard_Int_Dialog_H
