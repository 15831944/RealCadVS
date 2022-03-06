#ifndef DIMENSIONTEXT_DIALOG_H
#define DIMENSIONTEXT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class DimensionText_Dialog;
}

class DimensionText_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DimensionText_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DimensionText_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionText_Dialog();

signals:
    
public slots:
    void on_accept();


private:
    Ui::DimensionText_Dialog *ui;
};

#endif // DIMENSIONTEXT_DIALOG_H
