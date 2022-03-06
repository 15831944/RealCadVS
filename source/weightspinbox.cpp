
#include "ncwin.h"
#include "weightspinbox.h"

WeightSpinBox::WeightSpinBox(QWidget *parent)
    : QDoubleSpinBox(parent)
{
    curId = 0;
}

QString	WeightSpinBox::textFromValue(double value) const
{
    int w = db.lineweights.mmtoweight(value);
    return QString("%1").arg(db.lineweights.weighttomm(w),0,'f',2,'0');
}

double	WeightSpinBox::valueFromText(const QString & text) const
{
    int w = db.lineweights.mmtoweight(text.toDouble());
    return db.lineweights.weighttomm(w);
}

void WeightSpinBox::stepBy(int steps)
{
    //qDebug() << "stepBy" << steps;
    if(steps >0)
        curId++;
    else
        curId--;

    if(curId < 0)
        curId = 11;
    if(curId > 11)
        curId = 0;

    setValue(db.lineweights.weighttomm(curId));
}
