#ifndef RCONECOORDINATEENTRY_H
#define RCONECOORDINATEENTRY_H

#include <QWidget>
#include <QLineEdit>
#include <QIcon>
#include <QLabel>

namespace Ui {
class RCOneCoordinateEntry;
}

class RCOneCoordinateEntry : public QWidget
{
    Q_OBJECT
public:
    explicit RCOneCoordinateEntry(QWidget *parent = nullptr);

    QLabel * getPrompt();
    QLabel * getIcon();
    QLineEdit * getEdit();

signals:

public slots:

private:
    Ui::RCOneCoordinateEntry *ui;
};

#endif // RCCOORDINATEENTRY_H
