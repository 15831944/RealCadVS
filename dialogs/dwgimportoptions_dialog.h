#ifndef DWGIMPORTOPTIONS_DIALOG_H
#define DWGIMPORTOPTIONS_DIALOG_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

namespace Ui {
class DWGImportOptions_Dialog;
}

class DWGImportOptions_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit DWGImportOptions_Dialog(QWidget *parent = 0);
    ~DWGImportOptions_Dialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DWGImportOptions_Dialog *ui;
};

#endif // DWGIMPORTOPTIONS_DIALOG_H
