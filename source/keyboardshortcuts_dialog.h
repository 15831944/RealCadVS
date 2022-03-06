#ifndef KEYBOARDSHORTCUT_DIALOG_H
#define KEYBOARDSHORTCUT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class KeyboardShortcuts_Dialog;
}

class KeyboardShortcuts_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit KeyboardShortcuts_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  KeyboardShortcuts_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~KeyboardShortcuts_Dialog();

signals:

public slots:
    void on__102_clicked(bool checked);
    void on__103_clicked(bool checked);
    void on__104_clicked(bool checked);
    void on_accept();

private:
    Ui::KeyboardShortcuts_Dialog *ui;
};

#endif // KEYBOARDSHORTCUT_DIALOG_H
