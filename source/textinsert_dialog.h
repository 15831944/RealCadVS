#ifndef TEXTINSERT_DIALOG_H
#define TEXTINSERT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class TextInsert_Dialog;
}

class TextInsert_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit TextInsert_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE TextInsert_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~TextInsert_Dialog();
    
signals:
    
public slots:
    void on__103_clicked(bool checked);
    void on_accept();

private:
    Ui::TextInsert_Dialog *ui;
};

#endif // TEXTINSERT_DIALOG_H
