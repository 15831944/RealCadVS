#ifndef BUMPMAP_DIALOG_H
#define BUMPMAP_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Bumpmap_Dialog;
}

class Bumpmap_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Bumpmap_Dialog(QWidget *parent = nullptr, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Bumpmap_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Bumpmap_Dialog();

signals:

public slots:
    void on__105_clicked();
    void on_accept();

private slots:
    void on__106_clicked();

private:
    Ui::Bumpmap_Dialog *ui;
};

#endif // BUMPMAP_DIALOG_H
