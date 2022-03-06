#ifndef TEXTVERIFY_DIALOG_H
#define TEXTVERIFY_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class TextVerify_Dialog;
}

class TextVerifyEventFilter : public QObject
{    Q_OBJECT
public:
    explicit TextVerifyEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class TextVerify_Dialog : public RCDialog
{
    friend class TextVerifyEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_100;
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_106;
    bool variesWasSet_107;
    bool variesWasSet_108;
    bool variesWasSet_111;
    bool variesWasSet_121;
    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit TextVerify_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE TextVerify_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~TextVerify_Dialog();

    void drawSample();

signals:
    
public slots:
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(double v);
    void on__105_currentFontChanged(const QFont & font);
    void on__106_valueChanged(double v);
    void on__107_valueChanged(double v);
    void on__108_valueChanged(double v);
    void on__111_valueChanged(double v);
    void on__109_currentIndexChanged(int index); // horizontal justification
    void on__110_currentIndexChanged(int index); // vertical justification
    void on__121_valueChanged(int v);

    void on__1000_clicked(bool checked);
    void on__1001_clicked(bool checked);
    void on__1002_clicked(bool checked);
    void on__1003_clicked(bool checked);
    void on__1004_clicked(bool checked);

    void on__112_clicked(bool checked);
    void on__113_clicked(bool checked);
    void on__114_clicked(bool checked);
    void on__115_clicked(bool checked);
    void on__116_clicked(bool checked);
    void on__120_clicked(bool checked);
    void on__122_clicked(bool checked);
    void on__123_clicked(bool checked);

    void on__117_clicked(bool checked);
    void on__118_clicked(bool checked);
    void on_accept();

private:
    Ui::TextVerify_Dialog *ui;
};

#endif // TEXTVERIFY_DIALOG_H
