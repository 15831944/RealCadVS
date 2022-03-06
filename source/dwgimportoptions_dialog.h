#ifndef DWGIMPORTOPTIONS_DIALOG_H
#define DWGIMPORTOPTIONS_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"
#include <QDialog>

namespace Ui {
class DWGImportOptions_Dialog;
}

class DWGImportOptions_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit DWGImportOptions_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE DWGImportOptions_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DWGImportOptions_Dialog();

    void updateCheckBoxes();

protected:
    //void changeEvent(QEvent *e);

private slots:
    void on_accept();
    void on__100_clicked(bool checked);

    void on__200_clicked(bool checked);

private:
    Ui::DWGImportOptions_Dialog *ui;
};

#endif // DWGIMPORTOPTIONS_DIALOG_H
