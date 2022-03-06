#ifndef CALCULATOR_DIALOG_H
#define CALCULATOR_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Calculator_Dialog;
}

class Calculator_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Calculator_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Calculator_Dialog(HANDLE parent, HANDLE rcparent, int f);
     ~Calculator_Dialog();


signals:
    
public slots:
    //void on__100_selectionChanged();
    //void on__100_textEdited(const QString & text);
    void on__100_editingFinished();
    void on_key_pressed();
    void on_accept();

private:
    Ui::Calculator_Dialog *ui;
};

#endif // CALCULATOR_DIALOG_H
