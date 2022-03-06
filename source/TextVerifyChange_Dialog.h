#ifndef TEXTVERIFYCHANGE_DIALOG_H
#define TEXTVERIFYCHANGE_DIALOG_H

#include "ncwin.h"

#include "RCDialog.h"

namespace Ui {
class TextVerifyChange_Dialog;
}

class TextVerifyMousePressFilter : public QObject
{    Q_OBJECT
public:
    explicit TextVerifyMousePressFilter(QObject *parent=0) : QObject(parent) {; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class TextVerifyChange_Dialog : public RCDialog
{
    friend class TextVerifyMousePressFilter;

    Q_OBJECT
public:
    explicit TextVerifyChange_Dialog(QWidget *parent = 0, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE TextVerifyChange_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~TextVerifyChange_Dialog();

signals:
    
public slots:
    void on__100_valueChanged(int v);
    void on__101_valueChanged(int v);
    void on__102_valueChanged(int v);
    void on__103_valueChanged(int v);

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
    void on_accept();

private:
    Ui::TextVerifyChange_Dialog *ui;
};

#endif // TEXTVERIFYCHANGE_DIALOG_H
