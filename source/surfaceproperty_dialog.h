#ifndef SURFACEPROPERTY_DIALOG_H
#define SURFACEPROPERTY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SurfaceProperty_Dialog;
}

class SurfaceProperty_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SurfaceProperty_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SurfaceProperty_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SurfaceProperty_Dialog();

signals:

public slots:
    void on__100_currentIndexChanged(int index);
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__114_clicked(bool checked);
    void on__115_clicked(bool checked);
    void on_accept();

private:
    Ui::SurfaceProperty_Dialog *ui;
};

#endif // SURFACEPROPERTY_DIALOG_H
