#include "dwgimportoptions_dialog.h"
#include "ui_dwgimportoptions_dialog.h"

DWGImportOptions_Dialog::DWGImportOptions_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DWGImportOptions_Dialog)
{
    ui->setupUi(this);
}

DWGImportOptions_Dialog::~DWGImportOptions_Dialog()
{
    delete ui;
}

void DWGImportOptions_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
