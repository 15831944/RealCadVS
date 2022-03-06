#ifndef DIMENSIONSUFFIX0_DIALOG_H
#define DIMENSIONSUFFIX0_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DimensionSuffix0_Dialog;
}

class DimensionSuffix0_Dialog : public RCDialog
{
    Q_OBJECT
protected:
    int changecount;
    bool change;
    BitMask * changeBits;
    bool ignoreCheckChange;
    bool ignoreTextChange;
    bool selectText;
    QString lastText;

public:
    explicit DimensionSuffix0_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DimensionSuffix0_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionSuffix0_Dialog();

    void drawSample();

signals:


public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_textChanged();
    void on__104_clicked(bool checked);
    void on_accept();

private:
    Ui::DimensionSuffix0_Dialog *ui;
};

#endif // DIMENSIONSUFFIX0_DIALOG_H
