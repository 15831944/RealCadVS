#ifndef DIMENSIONTERMINATOR_DIALOG_H
#define DIMENSIONTERMINATOR_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DimensionTerminator_Dialog;
}

class DimTerminatorVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit DimTerminatorVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class DimensionTerminator_Dialog : public RCDialog
{
    friend class DimTerminatorVerifyEventFilter;

    Q_OBJECT
public:
    explicit DimensionTerminator_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DimensionTerminator_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionTerminator_Dialog();

    void drawSample();

protected:
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_106;

    int changecount;
    bool change;
    BitMask * changeBits;

signals:


public slots:
    void on__100_currentIndexChanged(int index);
    void on__101_valueChanged(double v);
    void on__102_valueChanged(double v);
    void on__103_currentIndexChanged(int index);
    void on__104_currentIndexChanged(int index);
    void on__106_valueChanged(double v);
    void on__107_clicked(bool checked);
    void on__1006_clicked(bool checked);
    void on_accept();

private:
    Ui::DimensionTerminator_Dialog *ui;
};

#endif // DIMENSIONTERMINATOR_DIALOG_H
