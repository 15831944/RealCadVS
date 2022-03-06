
#include "ncwin.h"
#include <QKeyEvent>
#include <QDebug>
#include "rcdoublespinbox.h"

/**
 * @brief RCDoubleSpinBox::RCDoubleSpinBox
 * @param parent
 *
 * This widget is designed to work like the old RealCAD RealDialogControl
 * the input is evaluated for expressions when the focus leaves the control
 * but syntax errors are ignored. The owning dialog shows an error message box
 * and the user must fix the syntax errors or cancel the dialog
 *
 * dialogs that use this control should check the return value from dialogcb()
 * and close the dialog if true.
 *
 */
RCDoubleSpinBox::RCDoubleSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent), defaultValue(0.0)
{
}

double RCDoubleSpinBox::evaluate(const QString & text) const
{
    RCCHAR string[300];
    double x;
    strcpy(string,text.data());
    Expression expression(string);
    if(expression.evaluate(&x) != 0)
        return x;

    return NAN;
}

QAbstractSpinBox::StepEnabled RCDoubleSpinBox::stepEnabled() const
{
    bool ok;
    QString val = lineEdit()->text();
    val.toDouble(&ok);
    if(!ok)
        return QAbstractSpinBox::StepNone;
    else
        return QDoubleSpinBox::stepEnabled();
}

void RCDoubleSpinBox::stepBy(int steps)
{
    QDoubleSpinBox::stepBy(steps);
}

QValidator::State RCDoubleSpinBox::validate(QString & text, int & pos) const
{
    // validation is done later
    // assume all input is OK here
    return QValidator::Acceptable;
}

void RCDoubleSpinBox::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        QString text = cleanText();
        double val = evaluate(text);
#ifdef isnan
        if(isnan(val))
#else
        if(std::isnan(val))
#endif
        {
            //ResourceString rs1(NCDIALOG+1);
            //QMessageBox::warning(0,"Input Error",QString(rs1.gets())+"\r"+text);
            setSpecialValueText(text);
            event->accept();
        }
        else
        {
            setValue(val);
            event->accept();
        }
    }
    QDoubleSpinBox::keyPressEvent(event);
}

void RCDoubleSpinBox::focusOutEvent(QFocusEvent * event)
{

    QString text = cleanText();
    double val = evaluate(text);
#ifdef isnan
    if(isnan(val))
#else
    if(std::isnan(val))
#endif
    {
        //ResourceString rs1(NCDIALOG+1);
        //QMessageBox::warning(0,"Input Error",QString(rs1.gets())+"\r"+text);
        setSpecialValueText(text);
        event->accept();
    }
    else
    {
        setValue(val);
        event->accept();
    }

    QDoubleSpinBox::focusOutEvent(event);
}

QString RCDoubleSpinBox::textFromValue(double value) const
{
    return QString::number(value);
}

double RCDoubleSpinBox::valueFromText(const QString & text) const
{
    double val = evaluate(text);
#ifdef isnan
    if(isnan(val))
#else
    if(std::isnan(val))
#endif
    {
        if(specialValueText() == "*Varies*")
            return minimum();
        else
            return defaultValue;
    }
    else
        return val;

}
