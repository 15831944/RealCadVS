#ifndef StandardString_Dialog_H
#define StandardString_Dialog_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class StandardString_Dialog;
}

class StandardString_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit StandardString_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE StandardString_Dialog(HANDLE parent, HANDLE rcparent, int f);
     ~StandardString_Dialog();

signals:
    
public slots:
    void on_accept( );

private:
    Ui::StandardString_Dialog *ui;
};

#endif // StandardString_Dialog_H
