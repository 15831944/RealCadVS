#ifndef DIMENSIONVALUE_DIALOG_H
#define DIMENSIONVALUE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DimensionValue_Dialog;
}

class DimValueTextEventFilter : public QObject
{    Q_OBJECT
public:
    explicit DimValueTextEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class DimensionValue_Dialog : public RCDialog
{
    friend class DimValueTextEventFilter;

    Q_OBJECT
public:
    explicit DimensionValue_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DimensionValue_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionValue_Dialog();

    void drawSample();

protected:
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_106;

    int changecount;
    bool change;
    BitMask * changeBits;

signals:

public slots:
    void on__100_currentFontChanged(const QFont & font);
    void on__101_valueChanged(double v);
    void on__102_valueChanged(double v);
    void on__103_valueChanged(double v);
    void on__104_currentIndexChanged(int index);
    void on__105_currentIndexChanged(int index);
    void on__106_valueChanged(double v);
    void on__107_clicked(bool checked);
    void on__108_clicked(bool checked);
    void on__109_clicked(bool checked);
    void on__110_clicked(bool checked);
    void on__111_clicked(bool checked);
    void on__113_currentIndexChanged(int index);
    void on_accept();


private:
    Ui::DimensionValue_Dialog *ui;
};

#endif // DIMENSIONVALUE_DIALOG_H
