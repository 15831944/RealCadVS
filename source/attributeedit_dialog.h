#ifndef ATTRIBUTEEDIT_DIALOG_H
#define ATTRIBUTEEDIT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class AttributeEdit_Dialog;
}

class AttributeEdit_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit AttributeEdit_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  AttributeEdit_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~AttributeEdit_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on_accept();

private:
    Ui::AttributeEdit_Dialog *ui;
};

#endif // ATTRIBUTEEDIT_DIALOG_H
