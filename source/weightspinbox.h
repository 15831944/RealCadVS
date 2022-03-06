#ifndef WEIGHTSPINBOX_H
#define WEIGHTSPINBOX_H

#include <QDoubleSpinBox>

class WeightSpinBox : public QDoubleSpinBox
{
        Q_OBJECT

    int curId;
public:
    explicit WeightSpinBox(QWidget *parent=0);

public slots:
    void stepBy(int steps);

protected:
    QString	textFromValue(double value) const ;
    double	valueFromText(const QString & text) const ;
};

#endif // WEIGHTSPINBOX_H
