#ifndef DIMENSIONCTERMINATOR_DIALOG_H
#define DIMENSIONCTERMINATOR_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DimensionCTerminator_Dialog;
}

class DimCTerminatorVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit DimCTerminatorVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class DimensionCTerminator_Dialog : public RCDialog
{
    friend class DimCTerminatorVerifyEventFilter;

    Q_OBJECT
public:
    explicit DimensionCTerminator_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DimensionCTerminator_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionCTerminator_Dialog();

    void drawSample();

protected:
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_104;

    int changecount;
    bool change;
    BitMask * changeBits;

signals:

public slots:
    void on__100_currentIndexChanged(int index);
    void on__101_valueChanged(double v);
    void on__102_valueChanged(double v);
    void on__104_valueChanged(double v);
    void on__105_clicked(bool checked);
    void on__1007_clicked(bool checked);
    void on_accept();

private:
    Ui::DimensionCTerminator_Dialog *ui;
};

#endif // DIMENSIONCTERMINATOR_DIALOG_H
