#ifndef LICENSEKEY_DIALOG_H
#define LICENSEKEY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class LicenseKey_Dialog;
}

class LicenseKey_Dialog :  public RCDialog
{
        Q_OBJECT

public:
    explicit LicenseKey_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  LicenseKey_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LicenseKey_Dialog();

public slots:
    void on__100_clicked();
    void on__101_clicked();
    void on__102_clicked();

private:
    Ui::LicenseKey_Dialog *ui;
};

#endif // LICENSEKEY_DIALOG_H
