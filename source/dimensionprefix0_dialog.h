#ifndef DIMENSIONPREFIX0_DIALOG_H
#define DIMENSIONPREFIX0_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class DimensionPrefix0_Dialog;
}

class DimensionPrefix0_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DimensionPrefix0_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DimensionPrefix0_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DimensionPrefix0_Dialog();

    void drawSample();

protected:
    int changecount;
    bool change;
    BitMask * changeBits;
    bool ignoreCheckChange;
    bool ignoreTextChange;
    bool selectText;
    QString lastText;

signals:


public slots:
    void on__100_clicked(bool checked);
    void on__101_clicked(bool checked);
    void on__102_textChanged();
    void on__104_clicked(bool checked);
    void on_accept();

private:
    Ui::DimensionPrefix0_Dialog *ui;
};

#endif // DIMENSIONPREFIX0_DIALOG_H
