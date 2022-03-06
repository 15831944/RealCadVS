#ifndef SYSTEMPATH_DIALOG_H
#define SYSTEMPATH_DIALOG_H

#include <QDialog>
#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class SystemPath_Dialog;
}

class SystemPath_Dialog : public RCDialog
{
    Q_OBJECT

public:
    explicit SystemPath_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE SystemPath_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~SystemPath_Dialog();

public: // data members
    QTreeWidgetItem *mSelectedItem;
    int mSelectedColumn;

public slots:
    void on_accept();

private slots:
    void on__101_clicked();

    void on__102_clicked();

    void on__100_itemClicked(QTreeWidgetItem *item, int column);

    void on__100_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on__100_itemSelectionChanged();

private:
    Ui::SystemPath_Dialog *ui;
};

#endif // SYSTEMPATH_DIALOG_H
