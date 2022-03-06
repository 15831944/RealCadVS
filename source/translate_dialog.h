#ifndef TRANSLATE_DIALOG_H
#define TRANSLATE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Translate_Dialog;
}

class Translate_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Translate_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Translate_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Translate_Dialog();

signals:

public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on_accept();

private:
    Ui::Translate_Dialog *ui;
};

#endif // TRANSLATE_DIALOG_H
