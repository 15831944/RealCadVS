#ifndef RCDIALOG_H
#define RCDIALOG_H

//#include "ncwin.h"

#include <QDialog>

class Dialog;
class QWidget;
class QDialog;

// a base class for Qt derived classes
// this class handles the realcad Dialog class interaction
// use it as a base class in any Qt derived classes that need it
class RCDialog : public QDialog
{ 
    Q_OBJECT

protected:
    Dialog *RCdlg;
    virtual void focusOutEvent(QFocusEvent * event);
    virtual void focusInEvent(QFocusEvent * event);
	bool ActivateParent;
public:
    //RCDialog(Dialog *rcdlg=0);
    RCDialog(Dialog *rcdlg=0, QWidget *parent=0, Qt::WindowFlags f = 0);
    ~RCDialog();
	bool toActivateParent() {
		return ActivateParent;
	}
    void setRCDialog(Dialog *rcdlg);
    Dialog* rcDialog();
    void initDialog(QWidget *qdlg);

    QWidget* getControl(int id);
    QWidget* getControl(QString name);

public slots:
    virtual void accept();
    virtual void reject();
};

#endif // RCDIALOG_H
