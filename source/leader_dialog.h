#ifndef LEADER_DIALOG_H
#define LEADER_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Leader_Dialog;
}

class LeaderVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit LeaderVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Leader_Dialog : public RCDialog
{
    friend class LeaderVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_105;
    bool variesWasSet_106;
    bool variesWasSet_109;
    bool variesWasSet_107;
    bool variesWasSet_118;
    bool variesWasSet_120;
    bool variesWasSet_124;
    bool variesWasSet_125;

    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit Leader_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  Leader_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Leader_Dialog();

signals:

public slots:
    void drawSample();
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(double v);
    void on__105_valueChanged(double v);

    void on__104_currentIndexChanged(int index);
    void on__108_currentIndexChanged(int index);
    void on__106_valueChanged(double v);
    void on__107_valueChanged(double v);
    void on__109_valueChanged(double v);
    void on__117_currentIndexChanged(int index);
    void on__118_valueChanged(double v);
    void on__120_valueChanged(double v);
    void on__124_valueChanged(double v);
    void on__125_valueChanged(int v);

    void on__1033_currentFontChanged(const QFont & font);

    void on__110_clicked(bool checked);// bold
    void on__111_clicked(bool checked);// underline
    void on__114_clicked(bool checked);// italic
    void on__115_clicked(bool checked);// boxed
    void on__112_clicked(bool checked);// outline
    void on__116_clicked(bool checked);// halo
    void on__121_clicked(bool checked);// scale text
    void on__119_clicked(bool checked);// specify terminator weight

    void on__122_clicked(bool checked);// clone button
    void on__123_clicked(bool checked);// clona all button

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on__1004_clicked(bool checked);
    void on__1005_clicked(bool checked);
    void on_accept();

private:
    Ui::Leader_Dialog *ui;
};

#endif // LEADER_DIALOG_H
