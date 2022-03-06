#ifndef TEXT_DIALOG_H
#define TEXT_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class Text_Dialog;
}

class TextDialogEventFilter : public QObject
{    Q_OBJECT
public:
    explicit TextDialogEventFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Text_Dialog : public RCDialog
{
    friend class TextDialogEventFilter;

    Q_OBJECT

protected:
    bool variesWasSet_101;
    bool variesWasSet_102;
    bool variesWasSet_103;
    bool variesWasSet_106;
    bool variesWasSet_116;
    QString defrs,varrs;
    int changecount;
    bool change;
    BitMask * changeBits;

public:
    explicit Text_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Text_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Text_Dialog();

    void drawSample();

signals:
    
public slots:
    void on__1000_activated(const QString & text);
    void on__1000_currentTextChanged(const QString & text);
    void on__1000_currentIndexChanged(int index);
    void on__100_activated(const QString & text);
    void on__100_currentFontChanged(const QFont & font);
    //void on__100_currentIndexChanged(int index);
    void on__1011_clicked(bool checked); // font combo undo button
    void on__101_valueChanged(double v); // text height
    void on__1001_clicked(bool checked); // text height undo button
    void on__102_valueChanged(double v); // text width
    void on__1002_clicked(bool checked); // text width undo button
    void on__103_valueChanged(double v); // text angle
    void on__1003_clicked(bool checked); // text angle undo button
    void on__104_currentIndexChanged(int index); // horizontal justification
    void on__1004_clicked(bool checked); // horizontal justification undo button
    void on__105_currentIndexChanged(int index); // vertical justification
    void on__1005_clicked(bool checked); // vertical justification undo button
    void on__106_valueChanged(double v); // vertical spacing
    void on__1006_clicked(bool checked); // vertical spacing undo button
    void on__107_clicked(bool checked); // bold
    void on__108_clicked(bool checked); // underline
    void on__109_clicked(bool checked); // italic
    void on__110_clicked(bool checked); // boxed
    void on__111_clicked(bool checked); // outline
    void on__112_clicked(bool checked); // scaled
    void on__114_clicked(bool checked); // clone button
    void on__115_clicked(bool checked); // halo
    void on__116_valueChanged(int v); // halo colour
    void on__1116_clicked(bool checked); // halo colour undo button
    void on__117_clicked(bool checked); // text height in points
    void on__118_clicked(bool checked); // clone all button
    void on__2000_clicked(bool checked); // colour selection dialog button
    void on_accept();

private:
    Ui::Text_Dialog *ui;
};

#endif // TEXT_DIALOG_H
