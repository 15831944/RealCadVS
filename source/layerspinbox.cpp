
#include "ncwin.h"
#include "layerspinbox.h"

LayerSpinBox::LayerSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
    curId = 0;
}

QString	LayerSpinBox::textFromValue(int value) const
{
    int layer = value;

    if(value < 0 )
        layer = MAX_LAYERS;
    if(value > MAX_LAYERS)
        layer = 0;

    QString str = QString("%1:%2")
            .arg(layer)
            .arg( (QString(db.layernames.getlayername(layer)) == 0 ? QString("") : QString(db.layernames.getlayername(layer))) );
    return str;
}

int LayerSpinBox::valueFromText(const QString & text) const
{
    return text.toInt();
}

void LayerSpinBox::stepBy(int steps)
{
    if(steps > 0)
        curId++;
    else
        curId--;

    if(curId < 0)
        curId = MAX_LAYERS;
    if(curId > MAX_LAYERS)
        curId = 0;

    setValue(curId);
}
