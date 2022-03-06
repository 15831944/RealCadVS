#ifndef SURFACEOFREVVERIFY_DIALOG_H
#define SURFACEOFREVVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SurfaceOfRevVerify_Dialog;
}

class SurfaceOfRevVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SurfaceOfRevVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SurfaceOfRevVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SurfaceOfRevVerify_Dialog();

signals:

public slots:
    void on__109_clicked(bool checked);
    void on__111_clicked(bool checked);
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on_accept();

private:
    Ui::SurfaceOfRevVerify_Dialog *ui;
};

#endif // SURFACEOFREVVERIFY_DIALOG_H
