#ifndef GRID_DIALOG_H
#define GRID_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

class GridDialogMousePressFilter : public QObject
{
    Q_OBJECT
public:
    explicit GridDialogMousePressFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class DefineColour_Dialog;
}

namespace Ui {
class Grid_Dialog;
}

class Grid_Dialog : public RCDialog
{
    Q_OBJECT
public:
    friend class GridDialogMousePressFilter;

    explicit Grid_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Grid_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Grid_Dialog();

    int style;
    int custcolour;

signals:

public slots:
    void on_accept( );
    void on__105_clicked(bool checked);
    void on__106_clicked(bool checked);
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on__1110_clicked(bool checked);
    void on__1130_clicked(bool checked);
    void on_styleCol_changed();

private:
    Ui::Grid_Dialog *ui;
};

#endif // GRID_DIALOG_H
