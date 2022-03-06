#ifndef ATTRIBUTEDELETE_DIALOG_H
#define ATTRIBUTEDELETE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class AttributeDelete_Dialog;
}

class AttributeDelete_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit AttributeDelete_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  AttributeDelete_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~AttributeDelete_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);

private:
    Ui::AttributeDelete_Dialog *ui;
};

#endif // ATTRIBUTEDELETE_DIALOG_H
