#ifndef LICENSEAGREEMENT_DIALOG_H
#define LICENSEAGREEMENT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class LicenseAgreement_Dialog;
}

class LicenseAgreement_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LicenseAgreement_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  LicenseAgreement_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LicenseAgreement_Dialog();

signals:

public slots:
    void on__101_clicked();
    void on__102_clicked();

private:
    Ui::LicenseAgreement_Dialog *ui;
};

#endif // LICENSEAGREEMENT_DIALOG_H
