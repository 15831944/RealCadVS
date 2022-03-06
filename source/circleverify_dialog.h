#ifndef CIRCLEVERIFY_DIALOG_H
#define CIRCLEVERIFY_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class CircleVerify_Dialog;
}

class CircleVerify_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit CircleVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE CircleVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~CircleVerify_Dialog();

signals:

public slots:
    void on__107_editingFinished();
    void on__111_editingFinished();

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);

    void on_accept();

private:
    Ui::CircleVerify_Dialog *ui;
};

#endif // CIRCLEVERIFY_DIALOG_H
