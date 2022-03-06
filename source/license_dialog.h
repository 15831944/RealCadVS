#ifndef LICENSE_DIALOG_H
#define LICENSE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class License_Dialog;
}

class License_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit License_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  License_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~License_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__103_clicked();

    void on__104_clicked(bool checked);
    void on__105_clicked(bool checked);
    void on__106_clicked();
    void on__108_clicked();
    void on__109_clicked();

    void on__111_clicked(bool checked);
    void on__112_clicked(bool checked);
    void on__113_clicked();

    void on_accept();

private:
    Ui::License_Dialog *ui;
};

#endif // LICENSE_DIALOG_H
