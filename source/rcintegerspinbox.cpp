
#include "ncwin.h"
#include <QKeyEvent>
#include <QDebug>
#include "rcintegerspinbox.h"

/**
 * @brief RCIntegerSpinBox::RCIntegerSpinBox
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
RCIntegerSpinBox::RCIntegerSpinBox(QWidget *parent) :
    QSpinBox(parent)
{

}

double RCIntegerSpinBox::evaluate(const QString & text) const
{
    RCCHAR string[300];
    double x;
    strcpy(string,text.data());
    Expression expression(string);
    if(expression.evaluate(&x) != 0)
        return x;

    return NAN;
}

QAbstractSpinBox::StepEnabled RCIntegerSpinBox::stepEnabled() const
{
    bool ok;
    QString val = lineEdit()->text();
    val.toInt(&ok);
    if(!ok)
        return QAbstractSpinBox::StepNone;
    else
        return QSpinBox::stepEnabled();
}

QValidator::State RCIntegerSpinBox::validate(QString & text, int & pos) const
{
    // validation is done later
    // assume all input is OK here
    return QValidator::Acceptable;
}

void RCIntegerSpinBox::keyPressEvent(QKeyEvent * event)
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
            setValue(int(val));
            event->accept();
        }
    }
    QSpinBox::keyPressEvent(event);
}

void RCIntegerSpinBox::focusOutEvent(QFocusEvent * event)
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
        setValue(int(val));
        event->accept();
    }
    QSpinBox::focusOutEvent(event);
}

QString RCIntegerSpinBox::textFromValue(int value) const
{
    return QString::number(value);
}

int RCIntegerSpinBox::valueFromText(const QString & text) const
{
    double val = evaluate(text);
#ifdef isnan
    if(isnan(val))
#else
    if(std::isnan(val))
#endif
        return minimum();
    else
        return int(val);
}
