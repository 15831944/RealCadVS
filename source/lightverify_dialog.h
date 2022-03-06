#ifndef LIGHT_DIALOG_H
#define LIGHT_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

class LightVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit LightVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

namespace Ui {
class LightVerify_Dialog;
}

class LightVerify_Dialog : public RCDialog
{
    friend LightVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_104;
    bool variesWasSet_105;
    bool variesWasSet_106;
    bool variesWasSet_107;
    bool variesWasSet_108;
    bool variesWasSet_109;
    bool variesWasSet_110;
    bool variesWasSet_111;
    bool variesWasSet_112;
    bool variesWasSet_113;
    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit LightVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE LightVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~LightVerify_Dialog();
    void draw();

    sunlightInfo *suninfo;

signals:

public slots:
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(double v);
    void on__104_valueChanged(double v);
    void on__107_valueChanged(double v);
    void on__109_valueChanged(double v);
    void on__110_valueChanged(double v);
    void on__112_valueChanged(double v);
    void on__113_valueChanged(double v);

    void on__105_clicked(bool checked);
    void on__106_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__111_clicked(bool checked);

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);

    void on_accept();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::LightVerify_Dialog *ui;
};

#endif // LightVerify_Dialog_H
