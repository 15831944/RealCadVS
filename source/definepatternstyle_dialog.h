#ifndef DEFINEPATTERNSTYLE_DIALOG_H
#define DEFINEPATTERNSTYLE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

class PatternStyleDisplayDialogControl;

namespace Ui {
class DefinePatternStyle_Dialog;
}

class DefinePatternStyle_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit DefinePatternStyle_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE  DefinePatternStyle_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~DefinePatternStyle_Dialog();

    void drawSample();

signals:

public slots:
    void on__100_valueChanged(double v); // height
    void on__101_valueChanged(double v); // length scale
    void on__110_currentTextChanged(const QString & text); // name list box
    void on__110_currentIndexChanged(int index); // name list box
    void on__112_currentIndexChanged(int index); // size list box
    void on__107_clicked(bool checked); // browse button
    void on__120_clicked(bool checked); // update button
    void on__121_clicked(bool checked); // delete button
    void on__122_clicked(bool checked); // define pattern
    void on_accept();

private:
    Ui::DefinePatternStyle_Dialog *ui;
};

#endif // DEFINEPATTERNSTYLE_DIALOG_H
