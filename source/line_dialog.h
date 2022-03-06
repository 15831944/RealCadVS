#ifndef LINE_DIALOG_H
#define LINE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Line_Dialog;
}

class Line_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Line_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Line_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Line_Dialog();

signals:

public slots:
    void on_accept();

private:
    Ui::Line_Dialog *ui;
};

#endif // LINE_DIALOG_H
