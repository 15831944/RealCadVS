#ifndef COMPOSEDDRAWING_DIALOG_H
#define COMPOSEDDRAWING_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class ComposedDrawing_Dialog;
}

class ComposedDrawing_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ComposedDrawing_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  ComposedDrawing_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ComposedDrawing_Dialog();

signals:

public slots:
    void on__101_currentIndexChanged(int index);
    void on__105_clicked(bool checked);
    void on_accept();

private:
    Ui::ComposedDrawing_Dialog *ui;
};

#endif // COMPOSEDDRAWING_DIALOG_H
