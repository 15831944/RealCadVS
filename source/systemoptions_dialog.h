#ifndef SYSTEMOPTIONSDIALOG_H
#define SYSTEMOPTIONSDIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class SystemOptions_Dialog;
}

class SystemOptions_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SystemOptions_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SystemOptions_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SystemOptions_Dialog();

signals:

public slots:
    void on__106_valueChanged(double v);
    void on__107_valueChanged(double v);
    void on__108_valueChanged(double v);
    void on__1000_clicked(bool checked);
    void on__150_valueChanged(double v);
    void on__151_valueChanged(double v);
    void on__152_valueChanged(double v);
    void on__1001_clicked(bool checked);
    void on__110_clicked(bool checked);
    void on__111_clicked(bool checked);
    void on__118_clicked(bool checked);
    void on__120_clicked(bool checked);
    void on__122_clicked(bool checked);
    void on__129_clicked(bool checked);
    void on__124_clicked(bool checked);
    void on__125_clicked(bool checked);
    void on__126_clicked(bool checked);
    void on__132_clicked(bool checked);
    void on__133_clicked(bool checked);
    void on__134_clicked(bool checked);
    void on__135_clicked(bool checked);
    void on__136_clicked(bool checked);
    void on__138_clicked(bool checked);
    void on__139_clicked(bool checked);
    void on__140_clicked();
    void on__143_clicked();
    void on_accept();

private slots:

private:
    Ui::SystemOptions_Dialog *ui;
};

#endif // SYSTEMOPTIONSDIALOG_H
