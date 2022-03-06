#ifndef FINDANDREPLACE_DIALOG_H
#define FINDANDREPLACE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class FindAndReplace_Dialog;
}

class FindAndReplace_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit FindAndReplace_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  FindAndReplace_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~FindAndReplace_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);// replace cb
    void on__103_clicked(bool checked);// search text cb
    void on__104_clicked(bool checked);// match case cb
    void on__105_clicked(bool checked);// seatch attributes cb
    void on__106_clicked(bool checked);// fit window cb
    void on__107_clicked(bool checked);// search figure names cb
    void on__108_clicked(bool checked);// search group class cb
    void on__109_clicked(bool checked);// search group instance cb
    void on__110_clicked(bool checked);// patternfill names cb
    void on__111_clicked(bool checked);// bitmapfill names cb
    void on__112_clicked(bool checked);// bitmap names cb
    void on__113_clicked(bool checked);// material texture names cb
    void on__114_clicked(bool checked);// bumpap texture names cb

    void on__1000_clicked(bool checked);// browse for find files button
    void on__1001_clicked(bool checked);// browse for replace files button

    void on_accept();

private:
    Ui::FindAndReplace_Dialog *ui;
};

#endif // FINDANDREPLACE_DIALOG_H
