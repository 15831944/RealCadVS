#ifndef SELECTDRAWINGVIEW_DIALOG_H
#define SELECTDRAWINGVIEW_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SelectDrawingView_Dialog;
}

class SelectDrawingView_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit SelectDrawingView_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  SelectDrawingView_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SelectDrawingView_Dialog();

    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__105_clicked(bool checked);
    void on__100_currentRowChanged(int currentRow);
    void on__101_currentRowChanged(int currentRow);
    void on__101_itemClicked(QListWidgetItem * item);
    void on_accept();

private:
    Ui::SelectDrawingView_Dialog *ui;
};

#endif // SELECTDRAWINGVIEW_DIALOG_H
