#ifndef TEXTURE_DIALOG_H
#define TEXTURE_DIALOG_H

#include "ncwin.h"
#include "RCDialog.h"

namespace Ui {
class Texture_Dialog;
}


class Texture_Dialog : public RCDialog
{
    Q_OBJECT
public:
    explicit Texture_Dialog(QWidget *parent = nullptr, Dialog *rcparent=0, Qt::WindowFlags f=0);
	Q_INVOKABLE Texture_Dialog(HANDLE parent, HANDLE rcparent, int f);
    ~Texture_Dialog();

    void drawSample();
    void PreviewImage(QWidget *widget, RCCHAR *filepath );

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:

public slots:
    void on_accept();

private slots:
    void on__100_currentIndexChanged(int index);
    void on__112_clicked();
    void on__117_clicked();

    void on__107_clicked(bool checked);

    void on__119_clicked(bool checked);

private:
    Ui::Texture_Dialog *ui;
};

#endif // TEXTURE_DIALOG_H
