#ifndef ATTRIBUTELIST_DIALOG_H
#define ATTRIBUTELIST_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class AttributeList_Dialog;
}

class AttributeList_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit AttributeList_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  AttributeList_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~AttributeList_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);

private:
    Ui::AttributeList_Dialog *ui;
};

#endif // ATTRIBUTELIST_DIALOG_H
