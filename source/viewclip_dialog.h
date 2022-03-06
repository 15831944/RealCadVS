#ifndef VIEWCLIP_DIALOG_H
#define VIEWCLIP_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class ViewClip_Dialog;
}

class ViewClip_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit ViewClip_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE ViewClip_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~ViewClip_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on_accept();

private:
    Ui::ViewClip_Dialog *ui;
};

#endif // VIEWCLIP_DIALOG_H
