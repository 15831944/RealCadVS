#ifndef TEXTINSERTCOUNTER_DIALOG_H
#define TEXTINSERTCOUNTER_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class TextInsertCounter_Dialog;
}

class TextInsertCounter_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit TextInsertCounter_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE TextInsertCounter_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~TextInsertCounter_Dialog();
    
signals:
    
public slots:
    void on_accept();

private:
    Ui::TextInsertCounter_Dialog *ui;
};

#endif // TEXTINSERTCOUNTER_DIALOG_H
