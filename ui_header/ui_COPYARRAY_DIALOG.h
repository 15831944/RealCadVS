/********************************************************************************
** Form generated from reading UI file 'COPYARRAY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYARRAY_DIALOG_H
#define UI_COPYARRAY_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_CopyArray_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QRadioButton *_100;
    QRadioButton *_101;
    QGroupBox *groupBox;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCIntegerSpinBox *_104;
    RCIntegerSpinBox *_105;
    QGroupBox *groupBox_2;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    RCIntegerSpinBox *_108;
    RCIntegerSpinBox *_106;
    RCIntegerSpinBox *_107;

    void setupUi(QDialog *CopyArray_Dialog)
    {
        if (CopyArray_Dialog->objectName().isEmpty())
            CopyArray_Dialog->setObjectName(QString::fromUtf8("CopyArray_Dialog"));
        CopyArray_Dialog->resize(311, 410);
        CopyArray_Dialog->setMinimumSize(QSize(311, 410));
        CopyArray_Dialog->setMaximumSize(QSize(311, 410));
        QFont font;
        font.setPointSize(10);
        CopyArray_Dialog->setFont(font);
        _2 = new QPushButton(CopyArray_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(222, 360, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CopyArray_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(132, 360, 75, 23));
        _100 = new QRadioButton(CopyArray_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(10, 10, 211, 20));
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(CopyArray_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(10, 32, 211, 20));
        _101->setLayoutDirection(Qt::RightToLeft);
        groupBox = new QGroupBox(CopyArray_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 60, 291, 151));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 120, 101, 21));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 92, 101, 21));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 56, 101, 21));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 30, 101, 21));
        _102 = new RCDoubleSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(178, 28, 99, 24));
        _102->setMinimum(-1000000000.000000000000000);
        _102->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(178, 54, 99, 24));
        _103->setMinimum(-1000000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _104 = new RCIntegerSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(178, 90, 99, 24));
        _104->setMinimum(1);
        _104->setMaximum(100000);
        _105 = new RCIntegerSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(178, 116, 99, 24));
        _105->setMinimum(1);
        _105->setMaximum(100000);
        groupBox_2 = new QGroupBox(CopyArray_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 230, 291, 121));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 90, 151, 21));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 60, 155, 21));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 30, 157, 21));
        _108 = new RCIntegerSpinBox(groupBox_2);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(178, 88, 99, 24));
        _108->setMinimum(1);
        _108->setMaximum(100000);
        _106 = new RCIntegerSpinBox(groupBox_2);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(178, 28, 99, 24));
        _106->setMinimum(-100000000);
        _106->setMaximum(100000000);
        _107 = new RCIntegerSpinBox(groupBox_2);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(178, 58, 99, 24));
        _107->setMinimum(-100000000);
        _107->setMaximum(100000000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(CopyArray_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CopyArray_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CopyArray_Dialog)
    {
        CopyArray_Dialog->setWindowTitle(QCoreApplication::translate("CopyArray_Dialog", "Copy array options", nullptr));
        _2->setText(QCoreApplication::translate("CopyArray_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CopyArray_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("CopyArray_Dialog", "Rectangular array", nullptr));
        _101->setText(QCoreApplication::translate("CopyArray_Dialog", "Circular array", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CopyArray_Dialog", "Rectangular array", nullptr));
        label_4->setText(QCoreApplication::translate("CopyArray_Dialog", "Number Y", nullptr));
        label_3->setText(QCoreApplication::translate("CopyArray_Dialog", "Number X", nullptr));
        label_5->setText(QCoreApplication::translate("CopyArray_Dialog", "Y spacing", nullptr));
        label_6->setText(QCoreApplication::translate("CopyArray_Dialog", "X spacing", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CopyArray_Dialog", "Circular array", nullptr));
        label_8->setText(QCoreApplication::translate("CopyArray_Dialog", "Number of circles", nullptr));
        label_9->setText(QCoreApplication::translate("CopyArray_Dialog", "Spacing around circles", nullptr));
        label_10->setText(QCoreApplication::translate("CopyArray_Dialog", "Spacing between circles", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CopyArray_Dialog: public Ui_CopyArray_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYARRAY_DIALOG_H
