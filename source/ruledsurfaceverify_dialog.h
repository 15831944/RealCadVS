#ifndef RULEDSURFACEVERIFY_DIALOG_H
#define RULEDSURFACEVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class RuledSurfaceVerify_Dialog;
}

class RuledSurfaceVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit RuledSurfaceVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE RuledSurfaceVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~RuledSurfaceVerify_Dialog();

signals:

public slots:
    void on__107_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on_accept();

private:
    Ui::RuledSurfaceVerify_Dialog *ui;
};

#endif // RULEDSURFACEVERIFY_DIALOG_H
