#ifndef RCPLANESAMPLELABEL_H
#define RCPLANESAMPLELABEL_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>

class RCPlaneSampleLabel : public QLabel
{
    Q_OBJECT

    double mDimHeight;

public:
    explicit RCPlaneSampleLabel(QWidget *parent = nullptr);
    void setDimHeight(double h);

protected:
    virtual void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // RCPLANESAMPLELABEL_H
