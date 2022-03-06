
#include "ncwin.h"
#include "ncdialog.h"
#include "RCDialog.h"
#include "systempath_dialog.h"
#include "ui_systempath_dialog.h"

SystemPath_Dialog::SystemPath_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SystemPath_Dialog((QWidget *)parent, (Dialog *)rcparent, (Qt::WindowFlags) f)
{}
SystemPath_Dialog::SystemPath_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SystemPath_Dialog)
{
    ui->setupUi(this);

    initDialog(this);

    QTreeWidget *treeWidget = ui->_100;
    if(treeWidget != 0)
    {
        QStringList labels;
        labels << "Components" << "Title Sheets" << "Template Drawings" << "Images" << "Hatch Patterns" << "Textures"
               << "Line Styles" << "My Drawings" << "DWG Preprocessor Application (Windows only option)" << "SketchUp skp Download" << "SketchUp Textures";

        // get the paths
        QSettings RCSettings(QString(home.getinifilename()),QSettings::IniFormat);
        QStringList paths;
        paths << RCSettings.value("Strings/db::figures-components-path").toString();
        paths << RCSettings.value("Strings/db::title-sheets-path").toString();
        paths << RCSettings.value("Strings/db::template-drawings-path").toString();
        paths << RCSettings.value("Strings/db::picture-images-path").toString();
        paths << RCSettings.value("Strings/db::hatch-patterns-path").toString();
        paths << RCSettings.value("Strings/db::textures-path").toString();
        paths << RCSettings.value("Strings/db::line-styles-path").toString();
        paths << RCSettings.value("Strings/db::my-drawings-path").toString();
        paths << RCSettings.value("Strings/db::dwg-preprocessor-path").toString();
        // sketchUp paths
        QSettings SkSettings(QString(home.getpublicfilename("RealCADSketchUp.ini")),QSettings::IniFormat);
        paths << SkSettings.value("RealCADSketchupInterface/DownloadDirectory","").toString();
        paths << SkSettings.value("RealCADSketchupInterface/TextureDirectory","").toString();

        // load the tree widget
        treeWidget->setColumnCount(1);
        QTreeWidgetItem *item,*subItem;

        for(int i=0; i<labels.size(); i++)
        {
            item = new QTreeWidgetItem(treeWidget);
            item->setText(0,labels.at(i));
            subItem = new QTreeWidgetItem(item);
            subItem->setText(0,paths.at(i));
            subItem->setFlags(subItem->flags() |= Qt::ItemIsEditable);
            treeWidget->addTopLevelItem(item);
        }
    }

    mSelectedItem=0;

    ui->_101->setEnabled(false);
    ui->_102->setEnabled(false);

    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
    connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

SystemPath_Dialog::~SystemPath_Dialog()
{
    delete ui;
}

void SystemPath_Dialog::on_accept()
{
    QTreeWidget *treeWidget = ui->_100;
    if(treeWidget != 0)
    {
        QSettings RCSettings(QString(home.getinifilename()),QSettings::IniFormat);
        int index=0;
        v.setstring("db::figures-components-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::figures-components-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::title-sheets-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::title-sheets-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::template-drawings-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::template-drawings-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::picture-images-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::picture-images-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::hatch-patterns-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::hatch-patterns-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::textures-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::textures-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::line-styles-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::line-styles-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::my-drawings-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::my-drawings-path",treeWidget->topLevelItem(index)->child(0)->text(0));
        // reset the db::last-cad-file-path to this path
        v.setstring("db::last-cad-file-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::last-cad-file-path",treeWidget->topLevelItem(index++)->child(0)->text(0));

        v.setstring("db::dwg-preprocessor-path",treeWidget->topLevelItem(index)->child(0)->text(0).data());
        RCSettings.setValue("Strings/db::dwg-preprocessor-path",treeWidget->topLevelItem(index++)->child(0)->text(0));
        RCSettings.sync();
        // sketchUp paths
        QSettings SkSettings(QString(home.getpublicfilename("RealCADSketchUp.ini")),QSettings::IniFormat);
        SkSettings.setValue("RealCADSketchupInterface/DownloadDirectory",treeWidget->topLevelItem(index++)->child(0)->text(0));
        SkSettings.setValue("RealCADSketchupInterface/TextureDirectory",treeWidget->topLevelItem(index++)->child(0)->text(0));
        SkSettings.sync();
    }

    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}

void SystemPath_Dialog::on__101_clicked()
{
    // don't do anything with parent items
    QList<QTreeWidgetItem *> itemList = ui->_100->selectedItems();
    if(itemList.isEmpty())
        return;
    QTreeWidgetItem *item = itemList.at(0);
    if(item == 0)
        return;
    if(item->parent() == 0)
        return;

    // select the new location
    ui->_101->setDefault(false);
//    QString newPath = QFileDialog::getExistingDirectory(Q_NULLPTR, "Select a File Location", QString(home.getpublicfilename("")), QFileDialog::ShowDirsOnly /*| QFileDialog::DontUseNativeDialog*/);
    QString newPath;
    if(mSelectedItem->parent()->text(0).contains("DWG Preprocessor"))
        newPath = QFileDialog::getOpenFileName(Q_NULLPTR, "Select the Executable File", QString(home.getpublicfilename("")),"Executable Files (*.exe)");
    else
        newPath = QFileDialog::getExistingDirectory(Q_NULLPTR, "Select a File Location", QString(home.getpublicfilename("")), QFileDialog::ShowDirsOnly /*| QFileDialog::DontUseNativeDialog*/);
    if(!newPath.isEmpty())
        mSelectedItem->setText(0,newPath);
    ui->_1->setDefault(true);
}

void SystemPath_Dialog::on__102_clicked()
{
    // reset to the standard location
    // find the selected top level item
    QList<QTreeWidgetItem *> itemList = ui->_100->selectedItems();
    if(itemList.isEmpty())
        return;
    QTreeWidgetItem *item = itemList.at(0);
    if(item == 0)
        return;
    if(item->parent() != 0)
    {
        // child item get the parent
        item = item->parent();
    }
    // get the correct default path depending on the item selected
    QSettings RCSettings(QString(home.getinifilename()),QSettings::IniFormat);
    QSettings SkSettings(QString(home.getpublicfilename("RealCADSketchUp.ini")),QSettings::IniFormat);

    QString pathName = item->text(0);
    if(pathName.contains("Components"))
    {
        QString path(home.getFigureFilepath(_RCT("")));
        //v.setstring("db::figures-components-path",path.data());
        path = QDir::toNativeSeparators(path);
        RCSettings.setValue("Strings/db::figures-components-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Title Sheets"))
    {
        QString path(home.getpublicfilename(_RCT("Title-Sheets")));
        //v.setstring("db::title-sheets-path",path.data());
        RCSettings.setValue("Strings/db::title-sheets-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Template Drawings"))
    {
        QString path(home.getpublicfilename(_RCT("Template-Drawings")));
        //v.setstring("db::template-drawings-path",path.data());
        RCSettings.setValue("Strings/db::template-drawings-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Images"))
    {
        QString path(home.getpublicfilename(_RCT("Images")));
        //v.setstring("db::picture-images-path",path.data());
        RCSettings.setValue("Strings/db::picture-images-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Hatch Patterns"))
    {
        QString path(home.getpublicfilename(_RCT("Hatch-Patterns")));
        //v.setstring("db::hatch-patterns-path",path.data());
        RCSettings.setValue("Strings/db::hatch-patterns-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Textures"))
    {
        QString path(home.getpublicfilename(_RCT("Textures")));
        //v.setstring("db::textures-path",path.data());
        RCSettings.setValue("Strings/db::textures-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("Line Styles"))
    {
        QString path(home.getpublicfilename(_RCT("Line-Styles")));
        //v.setstring("db::line-styles-path",path.data());
        RCSettings.setValue("Strings/db::line-styles-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("My Drawings"))
    {
        QString path(home.getpublicfilename(_RCT("My-Drawings")));
        if(path.isEmpty())
            path = QDir::homePath();
        //v.setstring("db::my-drawings-path",path.data());
        RCSettings.setValue("Strings/db::my-drawings-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.toLower().contains("DWG Preprocessor"))
    {
        QString path(home.getpublicfilename(_RCT("")));
        //v.setstring("db::dwg-preprocessor-path",path.data());
        RCSettings.setValue("Strings/db::dwg-preprocessor-path",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("SketchUp skp Download"))
    {
        QString path(home.getpublicfilename(_RCT("My-Drawings")));
        SkSettings.setValue("RealCADSketchupInterface/DownloadDirectory",path);
        item->child(0)->setText(0,path);
    }
    else if(pathName.contains("SketchUp Textures"))
    {
        QString path(home.getpublicfilename(_RCT("My-Drawings")));
        SkSettings.setValue("RealCADSketchupInterface/TextureDirectory",path);
        item->child(0)->setText(0,path);
    }
    RCSettings.sync();
    SkSettings.sync();
}

void SystemPath_Dialog::on__100_itemClicked(QTreeWidgetItem *item, int column)
{
    // don't do anything with parent items
    if(item->parent() == 0)
        return;

    ui->_101->setEnabled(true);
    ui->_102->setEnabled(true);
    ui->_101->setDefault(true);
    mSelectedItem = item;
    mSelectedColumn = column;
}

void SystemPath_Dialog::on__100_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // don't do anything with parent items
    if(item->parent() == 0)
        return;

    ui->_101->setEnabled(true);
    ui->_102->setEnabled(true);

    // select the new location
    ui->_101->setDefault(false);
    QString newPath;
    if(mSelectedItem->parent()->text(0).contains("DWG Preprocessor"))
        newPath = QFileDialog::getOpenFileName(Q_NULLPTR, "Select the Executable File", QString(home.getpublicfilename("")),"Executable Files (*.exe)");
    else
        newPath = QFileDialog::getExistingDirectory(Q_NULLPTR, "Select a File Location", QString(home.getpublicfilename("")), QFileDialog::ShowDirsOnly /*| QFileDialog::DontUseNativeDialog*/);
    if(!newPath.isEmpty())
        mSelectedItem->setText(0,QDir::toNativeSeparators(newPath));
    ui->_1->setDefault(true);
    ui->_101->setEnabled(false);
    ui->_102->setEnabled(false);
}

void SystemPath_Dialog::on__100_itemSelectionChanged()
{
    // find the selected top level item
    QTreeWidgetItem *item = ui->_100->selectedItems().at(0);
    if(item->parent() != 0)
    {
        // child item get the parent
        item = item->parent();
        mSelectedItem = item;
        mSelectedColumn = 0;
        ui->_101->setEnabled(true);
        ui->_102->setEnabled(true);
    }
}
