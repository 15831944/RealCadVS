#ifndef RAYTRACE_DIALOG_H
#define RAYTRACE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Raytrace_Dialog;
}

class Raytrace_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Raytrace_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Raytrace_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Raytrace_Dialog();

signals:

public slots:
    void on_accept();

private slots:
    void on__116_clicked();

    void on__117_clicked();

    void on__1000_clicked();

    void on__1001_clicked();

    void on__1002_clicked();

    void on__111_clicked(bool checked);

private:
    Ui::Raytrace_Dialog *ui;
};

#endif // RAYTRACE_DIALOG_H
