#ifndef SURFACEOFREVINSERT_DIALOG_H
#define SURFACEOFREVINSERT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SurfaceOfRevInsert_Dialog;
}

class SurfaceOfRevInsert_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SurfaceOfRevInsert_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SurfaceOfRevInsert_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SurfaceOfRevInsert_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::SurfaceOfRevInsert_Dialog *ui;
};

#endif // SURFACEOFREVINSERT_DIALOG_H
