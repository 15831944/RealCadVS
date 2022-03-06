/********************************************************************************
** Form generated from reading UI file 'LINEANGLE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEANGLE_DIALOG_H
#define UI_LINEANGLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_LineAngle_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_9;
    QLabel *label_10;
    QCheckBox *_101;
    QCheckBox *_103;
    QCheckBox *_104;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_102;

    void setupUi(QDialog *LineAngle_Dialog)
    {
        if (LineAngle_Dialog->objectName().isEmpty())
            LineAngle_Dialog->setObjectName(QString::fromUtf8("LineAngle_Dialog"));
        LineAngle_Dialog->resize(550, 120);
        LineAngle_Dialog->setMinimumSize(QSize(550, 120));
        LineAngle_Dialog->setMaximumSize(QSize(551, 120));
        QFont font;
        font.setPointSize(10);
        LineAngle_Dialog->setFont(font);
        _2 = new QPushButton(LineAngle_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(440, 70, 75, 23));
        _1 = new QPushButton(LineAngle_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(350, 70, 75, 23));
        label_9 = new QLabel(LineAngle_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 10, 55, 21));
        label_10 = new QLabel(LineAngle_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 40, 55, 21));
        _101 = new QCheckBox(LineAngle_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(223, 10, 123, 21));
        _101->setLayoutDirection(Qt::RightToLeft);
        _101->setCheckable(true);
        _101->setChecked(false);
        _101->setTristate(false);
        _103 = new QCheckBox(LineAngle_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(215, 40, 131, 21));
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _104 = new QCheckBox(LineAngle_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(true);
        _104->setGeometry(QRect(368, 10, 153, 21));
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setCheckable(true);
        _104->setChecked(false);
        _104->setTristate(false);
        _100 = new RCDoubleSpinBox(LineAngle_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(80, 10, 126, 22));
        _100->setDecimals(6);
        _100->setMaximum(360.000000000000000);
        _102 = new RCDoubleSpinBox(LineAngle_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(80, 40, 126, 22));
        _102->setDecimals(6);
        _102->setMaximum(10000.000000000000000);
        QWidget::setTabOrder(_100, _102);
        QWidget::setTabOrder(_102, _101);
        QWidget::setTabOrder(_101, _104);
        QWidget::setTabOrder(_104, _103);
        QWidget::setTabOrder(_103, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(LineAngle_Dialog);

        QMetaObject::connectSlotsByName(LineAngle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LineAngle_Dialog)
    {
        LineAngle_Dialog->setWindowTitle(QCoreApplication::translate("LineAngle_Dialog", "Insert lines at a given angle and length", nullptr));
        _2->setText(QCoreApplication::translate("LineAngle_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("LineAngle_Dialog", "OK", nullptr));
        label_9->setText(QCoreApplication::translate("LineAngle_Dialog", "Angle", nullptr));
        label_10->setText(QCoreApplication::translate("LineAngle_Dialog", "Length", nullptr));
        _101->setText(QCoreApplication::translate("LineAngle_Dialog", "Reference line", nullptr));
        _103->setText(QCoreApplication::translate("LineAngle_Dialog", "Constrain length", nullptr));
        _104->setText(QCoreApplication::translate("LineAngle_Dialog", "Move reference line", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LineAngle_Dialog: public Ui_LineAngle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEANGLE_DIALOG_H
