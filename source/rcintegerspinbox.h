#ifndef RCINTEGERSPINBOX_H
#define RCINTEGERSPINBOX_H

#include <QSpinBox>

class RCIntegerSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit RCIntegerSpinBox(QWidget *parent = 0);

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void focusOutEvent(QFocusEvent * event);
    QAbstractSpinBox::StepEnabled  stepEnabled() const;

signals:

protected slots:
    QValidator::State validate(QString & text, int & pos) const;
    QString textFromValue(int value) const;
    int valueFromText(const QString & text) const;

private:
    double evaluate(const QString &text) const;
};

#endif // RCINTEGERSPINBOX_H
