#ifndef LTTextInsert_Dialog_H
#define LTTextInsert_Dialog_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class LTTextInsert_Dialog;
}

class LTTextInsert_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit LTTextInsert_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE LTTextInsert_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LTTextInsert_Dialog();

signals:
    
public slots:
    void on_accept();

private:
    Ui::LTTextInsert_Dialog *ui;
};

#endif // LTTextInsert_Dialog_H
