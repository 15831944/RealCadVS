#ifndef RESETWORKPLANE_DIALOG_H
#define RESETWORKPLANE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class ResetWorkplane_Dialog;
}

class ResetWorkplane_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ResetWorkplane_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE ResetWorkplane_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ResetWorkplane_Dialog();
    
signals:
    
public slots:
    void on_accept();

private:
    Ui::ResetWorkplane_Dialog *ui;
};

#endif // RESETWORKPLANE_DIALOG_H
