#ifndef NETWORKSOFTWARELICENSE_DIALOG_H
#define NETWORKSOFTWARELICENSE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class NetworkSoftwareLicense_Dialog;
}

class NetworkSoftwareLicense_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit NetworkSoftwareLicense_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  NetworkSoftwareLicense_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~NetworkSoftwareLicense_Dialog();

signals:

public slots:
    void on__100_clicked();
    void on__101_clicked();
    void on__102_clicked();

private:
    Ui::NetworkSoftwareLicense_Dialog *ui;
};

#endif // NETWORKSOFTWARELICENSE_DIALOG_H
