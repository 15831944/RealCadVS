#ifndef PRINTPREVIEW_DIALOG_H
#define PRINTPREVIEW_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class PrintPreview_Dialog;
}

//class PrintPreview_Dialog : public QDialog, public RCDialog
class PrintPreview_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit PrintPreview_Dialog(QWidget *parent=0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE PrintPreview_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~PrintPreview_Dialog();

signals:
    
public slots:
    void on__100_activated(int index);
#if MAJOR_VERSION > 7
    void on__104_activated(int index);
#endif
    void on_accept();

private:
    Ui::PrintPreview_Dialog *ui;
};

#endif // PRINTPREVIEW_DIALOG_H
