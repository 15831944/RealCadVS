#ifndef ATTRIBUTENEW_DIALOG_H
#define ATTRIBUTENEW_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class AttributeNew_Dialog;
}

class AttributeNew_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit AttributeNew_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  AttributeNew_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~AttributeNew_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::AttributeNew_Dialog *ui;
};

#endif // ATTRIBUTENEW_DIALOG_H
