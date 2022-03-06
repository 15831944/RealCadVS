/********************************************************************************
** Form generated from reading UI file 'REFERENCE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFERENCE_DIALOG_H
#define UI_REFERENCE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Reference_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_8;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_106;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *Reference_Dialog)
    {
        if (Reference_Dialog->objectName().isEmpty())
            Reference_Dialog->setObjectName(QString::fromUtf8("Reference_Dialog"));
        Reference_Dialog->resize(361, 210);
        Reference_Dialog->setMinimumSize(QSize(361, 210));
        Reference_Dialog->setMaximumSize(QSize(361, 210));
        Reference_Dialog->setSizeIncrement(QSize(361, 190));
        groupBox = new QGroupBox(Reference_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 341, 142));
        QFont font;
        font.setPointSize(10);
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(13, 22, 111, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(163, 22, 111, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(92, 78, 31, 20));
        label_9->setFont(font);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(92, 104, 31, 20));
        label_12->setFont(font);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(92, 52, 31, 20));
        label_8->setFont(font);
        _102 = new RCDoubleSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(134, 50, 100, 24));
        _102->setAccelerated(true);
        _102->setMinimum(-100000000.000000000000000);
        _102->setMaximum(1000000000.000000000000000);
        _102->setValue(0.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(134, 76, 100, 24));
        _104->setAccelerated(true);
        _104->setMinimum(-100000000.000000000000000);
        _104->setMaximum(1000000000.000000000000000);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(134, 102, 100, 24));
        _106->setAccelerated(true);
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _1 = new QPushButton(Reference_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(178, 156, 75, 23));
        _1->setAutoDefault(false);
        _2 = new QPushButton(Reference_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(272, 156, 75, 23));
        _2->setAutoDefault(false);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _104);
        QWidget::setTabOrder(_104, _106);
        QWidget::setTabOrder(_106, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Reference_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Reference_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Reference_Dialog)
    {
        Reference_Dialog->setWindowTitle(QCoreApplication::translate("Reference_Dialog", "Reference from position", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Reference_Dialog", "New Coordinates", nullptr));
        _100->setText(QCoreApplication::translate("Reference_Dialog", "Cartesian", nullptr));
        _101->setText(QCoreApplication::translate("Reference_Dialog", "Cylindrical", nullptr));
        label_9->setText(QCoreApplication::translate("Reference_Dialog", "Y", nullptr));
        label_12->setText(QCoreApplication::translate("Reference_Dialog", "Z", nullptr));
        label_8->setText(QCoreApplication::translate("Reference_Dialog", "X", nullptr));
        _1->setText(QCoreApplication::translate("Reference_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Reference_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Reference_Dialog: public Ui_Reference_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFERENCE_DIALOG_H
