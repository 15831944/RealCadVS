#ifndef MEASURE_DIALOG_H
#define MEASURE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Measure_Dialog;
}

class Measure_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit Measure_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Measure_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Measure_Dialog();

protected:
    void closeEvent(QCloseEvent * e);

public slots:
    void on_hide();
    void on__107_clicked(bool checked);
    void on__120_clicked(bool checked);
    void on__121_clicked(bool checked);

private:
    Ui::Measure_Dialog *ui;
};

#endif // MEASURE_DIALOG_H
