#ifndef LAYERSPINBOX_H
#define LAYERSPINBOX_H

#include <QSpinBox>

class LayerSpinBox : public QSpinBox
{
    Q_OBJECT

    int curId;
public:
    explicit LayerSpinBox(QWidget *parent = 0);
    
signals:
    
public slots:
    void stepBy(int steps);

protected:
    QString	textFromValue(int value) const ;
    int	valueFromText(const QString & text) const ;
};

#endif // LAYERSPINBOX_H
