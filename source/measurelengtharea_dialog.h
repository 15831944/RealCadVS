#ifndef MEASURELENGTHAREA_DIALOG_H
#define MEASURELENGTHAREA_DIALOG_H


#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class MeasureLengthArea_Dialog;
}

class MeasureLengthArea_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit MeasureLengthArea_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE MeasureLengthArea_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~MeasureLengthArea_Dialog();


protected:
    void closeEvent(QCloseEvent * e);

signals:

public slots:
    void on_hide();
    void on__110_clicked(bool checked);
    //void on_accept();

private:
    Ui::MeasureLengthArea_Dialog *ui;
};

#endif // MEASURELENGTHAREA_DIALOG_H
