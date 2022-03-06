#ifndef TRACE_DIALOG_H
#define TRACE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Trace_Dialog;
}

#if 1 // if not using realcad dialog stuff
class Trace_Dialog : public QDialog
#else
class Trace_Dialog : public RCDialog
#endif
{
    Q_OBJECT
public:
    explicit Trace_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Trace_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Trace_Dialog();

    QTimer *timer;

    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void gotYes();
    void gotNo();

public slots:
    void on__6_clicked(bool checked);
    void on__7_clicked(bool checked);

private:
    Ui::Trace_Dialog *ui;
	
};

#endif // TRACE_DIALOG_H
