#ifndef RCDOUBLESPINBOX_H
#define RCDOUBLESPINBOX_H

#include <QDoubleSpinBox>

class RCDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    double defaultValue;

public:
    RCDoubleSpinBox(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void focusOutEvent(QFocusEvent * event);
    QAbstractSpinBox::StepEnabled  stepEnabled() const;
    virtual void stepBy(int steps);

signals:

protected slots:
    QValidator::State validate(QString & text, int & pos) const;
    QString textFromValue(double value) const;
    double valueFromText(const QString & text) const;

private:
    double evaluate(const QString &text) const;
};

#endif // RCDOUBLESPINBOX_H
