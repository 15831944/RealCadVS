#ifndef FIGURETEXT_DIALOG_H
#define FIGURETEXT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class FigureText_Dialog;
}

class FigureText_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit FigureText_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE FigureText_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~FigureText_Dialog();

signals:

public slots:
     void on_accept();

private:
    Ui::FigureText_Dialog *ui;
};

#endif // FIGURETEXT_DIALOG_H
