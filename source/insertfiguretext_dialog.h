#ifndef INSERTFIGURETEXT_DIALOG_H
#define INSERTFIGURETEXT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class InsertFigureText_Dialog;
}

class InsertFigureText_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit InsertFigureText_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  InsertFigureText_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~InsertFigureText_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::InsertFigureText_Dialog *ui;
};

#endif // INSERTFIGURETEXT_DIALOG_H
