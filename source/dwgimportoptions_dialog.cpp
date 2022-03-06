
#include "ncwin.h"
#include "RCDialog.h"
#include "dwgimportoptions_dialog.h"
#include "ui_dwgimportoptions_dialog.h"

DWGImportOptions_Dialog::DWGImportOptions_Dialog(HANDLE parent, HANDLE rcparent, int f):
	DWGImportOptions_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
DWGImportOptions_Dialog::DWGImportOptions_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::DWGImportOptions_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    ui->_100->setChecked(settings.value("Integers/dg::in-use_basic_options",0).toInt()?true:false);
    ui->_101->setChecked(settings.value("Integers/dg::in-use_dwg_layers",0).toInt()?true:false);

    QString dcwinapp(v.getstring("db::dwg-preprocessor-path"));
    QFileInfo fileInfo(dcwinapp);
    ui->_200->setEnabled( !fileInfo.isDir() && fileInfo.exists(dcwinapp) );
    if(ui->_200->isEnabled())
        ui->_200->setChecked(settings.value("Integers/dg::in-use_advanced_options",0).toInt()?true:false);
    else
        ui->_200->setChecked(false);

    ui->_201->setChecked(settings.value("Integers/dg::in-purge_dwg",0).toInt()?true:false);
    ui->_202->setChecked(settings.value("Integers/dg::in-visible_layers_only",0).toInt()?true:false);
    ui->_203->setChecked(settings.value("Integers/dg::in-model_space_only",0).toInt()?true:false);
    ui->_204->setChecked(settings.value("Integers/dg::in-explode_blocks",0).toInt()?true:false);
    ui->_205->setChecked(settings.value("Integers/dg::in-xrefs_to_blocks",0).toInt()?true:false);
    ui->_206->setChecked(settings.value("Integers/dg::in-polylines_to_lines",0).toInt()?true:false);
    ui->_207->setChecked(settings.value("Integers/dg::in-proxy_to_blocks",0).toInt()?true:false);
    ui->_208->setChecked(settings.value("Integers/dg::in-explode_hatch_patterns",0).toInt()?true:false);

    updateCheckBoxes();

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    // hide the advanced option check boxes on the mac
#ifdef _MAC
    ui->_201->hide();
    ui->_202->hide();
    ui->_203->hide();
    ui->_204->hide();
    ui->_205->hide();
    ui->_206->hide();
    ui->_207->hide();
    ui->_208->hide();
    ui->gridLayout->removeItem(ui->horizontalSpacer_201);
    ui->gridLayout->removeItem(ui->horizontalSpacer_202);
    ui->gridLayout->removeItem(ui->horizontalSpacer_203);
    ui->gridLayout->removeItem(ui->horizontalSpacer_204);
    ui->gridLayout->removeItem(ui->horizontalSpacer_205);
    ui->gridLayout->removeItem(ui->horizontalSpacer_206);
    ui->gridLayout->removeItem(ui->horizontalSpacer_207);
    ui->gridLayout->removeItem(ui->horizontalSpacer_208);
    ui->gridLayout->removeWidget(ui->_201);
    ui->gridLayout->removeWidget(ui->_202);
    ui->gridLayout->removeWidget(ui->_203);
    ui->gridLayout->removeWidget(ui->_204);
    ui->gridLayout->removeWidget(ui->_205);
    ui->gridLayout->removeWidget(ui->_206);
    ui->gridLayout->removeWidget(ui->_207);
    ui->gridLayout->removeWidget(ui->_208);
    // calculate a reasonable collapsed height and lock it
    int height = ui->_100->height() + ui->_101->height() + ui->_200->height();
    height += (ui->gridLayout->contentsMargins().top() + ui->gridLayout->contentsMargins().top()) * 6;
    height += ui->verticalSpacer->sizeHint().height();
    height += ui->_1->height() + ui->horizontalLayout->contentsMargins().top() + ui->horizontalLayout->contentsMargins().bottom();
    height += ui->verticalLayout_2->contentsMargins().top() + ui->verticalLayout_2->contentsMargins().bottom();
    this->setMinimumHeight(height);
    this->setMaximumHeight(height);
    this->adjustSize();
#endif
}

DWGImportOptions_Dialog::~DWGImportOptions_Dialog()
{
    delete ui;
}

void DWGImportOptions_Dialog::on_accept()
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    // save the settings
    settings.setValue("Integers/dg::in-use_basic_options",ui->_100->isChecked() ? 1 : 0);
    v.setinteger("dg::in-use_basic_options",ui->_100->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-use_dwg_layers",ui->_101->isChecked() ? 1 : 0);
    v.setinteger("dg::in-use_dwg_layers",ui->_101->isChecked() ? 1 : 0);

    settings.setValue("Integers/dg::in-use_advanced_options",ui->_200->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-purge_dwg",ui->_201->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-visible_layers_only",ui->_202->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-model_space_only",ui->_203->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-explode_blocks",ui->_204->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-xrefs_to_blocks",ui->_205->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-polylines_to_lines",ui->_206->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-proxy_to_blocks",ui->_207->isChecked() ? 1 : 0);
    settings.setValue("Integers/dg::in-explode_hatch_patterns",ui->_208->isChecked() ? 1 : 0);
    settings.sync();

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;

    if(dialogcb((HWND)this,msg,wParam,lParam))
        QDialog::accept();
}

void DWGImportOptions_Dialog::updateCheckBoxes()
{
    ui->_101->setEnabled(ui->_100->isChecked());

    ui->_201->setEnabled(ui->_200->isChecked());
    ui->_202->setEnabled(ui->_200->isChecked());
    ui->_203->setEnabled(ui->_200->isChecked());
    ui->_204->setEnabled(ui->_200->isChecked());
    ui->_205->setEnabled(ui->_200->isChecked());
    ui->_206->setEnabled(ui->_200->isChecked());
    ui->_207->setEnabled(ui->_200->isChecked());
    ui->_208->setEnabled(ui->_200->isChecked());
}


/*
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
*/

void DWGImportOptions_Dialog::on__100_clicked(bool checked)
{
    updateCheckBoxes();
}

void DWGImportOptions_Dialog::on__200_clicked(bool checked)
{
   updateCheckBoxes();
}
