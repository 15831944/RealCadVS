#ifndef SUNPOSITION_DIALOG_H
#define SUNPOSITION_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SunPosition_Dialog;
}

class SunPosition_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SunPosition_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SunPosition_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SunPosition_Dialog();

signals:

public slots:
    void on__102_currentIndexChanged(int index);
    void on_accept();

private slots:
    void on__100_dateChanged(const QDate &date);

    void on__101_timeChanged(const QTime &time);


    void on__106_clicked();

    void on__108_clicked(bool checked);

private:
    Ui::SunPosition_Dialog *ui;
};

#endif // SUNPOSITION_DIALOG_H
