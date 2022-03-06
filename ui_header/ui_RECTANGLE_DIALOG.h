/********************************************************************************
** Form generated from reading UI file 'RECTANGLE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECTANGLE_DIALOG_H
#define UI_RECTANGLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Rectangle_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QGroupBox *groupBox_2;
    QLabel *label_10;
    QLabel *label_12;
    QCheckBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;
    QCheckBox *_105;
    QCheckBox *_106;

    void setupUi(QDialog *Rectangle_Dialog)
    {
        if (Rectangle_Dialog->objectName().isEmpty())
            Rectangle_Dialog->setObjectName(QString::fromUtf8("Rectangle_Dialog"));
        Rectangle_Dialog->resize(280, 312);
        Rectangle_Dialog->setMinimumSize(QSize(280, 312));
        Rectangle_Dialog->setMaximumSize(QSize(280, 312));
        QFont font;
        font.setPointSize(10);
        Rectangle_Dialog->setFont(font);
        _2 = new QPushButton(Rectangle_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(184, 264, 75, 23));
        _1 = new QPushButton(Rectangle_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(94, 264, 75, 23));
        groupBox = new QGroupBox(Rectangle_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 9, 251, 81));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 23, 181, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(20, 44, 181, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        groupBox_2 = new QGroupBox(Rectangle_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(16, 90, 251, 121));
        groupBox_2->setFont(font);
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(19, 80, 91, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(19, 53, 91, 20));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new QCheckBox(groupBox_2);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(20, 23, 191, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setAutoFillBackground(false);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        _103 = new RCDoubleSpinBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(130, 55, 96, 22));
        _103->setDecimals(6);
        _103->setMinimum(-10000000.000000000000000);
        _103->setMaximum(10000000.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(130, 80, 96, 22));
        _104->setDecimals(6);
        _104->setMinimum(-100000000.000000000000000);
        _104->setMaximum(100000000.000000000000000);
        _105 = new QCheckBox(Rectangle_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(10, 216, 141, 20));
        _105->setFont(font);
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setAutoFillBackground(false);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _106 = new QCheckBox(Rectangle_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(10, 240, 141, 20));
        _106->setFont(font);
        _106->setLayoutDirection(Qt::RightToLeft);
        _106->setAutoFillBackground(false);
        _106->setCheckable(true);
        _106->setChecked(false);
        _106->setTristate(false);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Rectangle_Dialog);

        QMetaObject::connectSlotsByName(Rectangle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Rectangle_Dialog)
    {
        Rectangle_Dialog->setWindowTitle(QCoreApplication::translate("Rectangle_Dialog", "Rectangle options", nullptr));
        _2->setText(QCoreApplication::translate("Rectangle_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Rectangle_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Rectangle_Dialog", "Insert rectangle using", nullptr));
        _100->setText(QCoreApplication::translate("Rectangle_Dialog", "Two corners", nullptr));
        _101->setText(QCoreApplication::translate("Rectangle_Dialog", "Two sides", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Rectangle_Dialog", "Translate by", nullptr));
        label_10->setText(QCoreApplication::translate("Rectangle_Dialog", "Height", nullptr));
        label_12->setText(QCoreApplication::translate("Rectangle_Dialog", "Width", nullptr));
        _102->setText(QCoreApplication::translate("Rectangle_Dialog", "Fixed width and height", nullptr));
        _105->setText(QCoreApplication::translate("Rectangle_Dialog", "Hatch", nullptr));
        _106->setText(QCoreApplication::translate("Rectangle_Dialog", "Square corners", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rectangle_Dialog: public Ui_Rectangle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECTANGLE_DIALOG_H
