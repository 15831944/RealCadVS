#ifndef PLOTSCALE_DIALOG_H
#define PLOTSCALE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class PlotScale_Dialog;
}

class PlotScale_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PlotScale_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PlotScale_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PlotScale_Dialog();

signals:

public slots:
    void on_accept();
    void on__100_currentIndexChanged(const QString & text);

private:
    Ui::PlotScale_Dialog *ui;
};

#endif // PLOTSCALE_DIALOG_H
