/********************************************************************************
** Form generated from reading UI file 'ROTATE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROTATE_DIALOG_H
#define UI_ROTATE_DIALOG_H

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

class Ui_Rotate_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_99;
    QRadioButton *_100;
    QRadioButton *_101;
    QRadioButton *_102;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox_2;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_103;
    RCIntegerSpinBox *_107;
    RCDoubleSpinBox *_106;

    void setupUi(QDialog *Rotate_Dialog)
    {
        if (Rotate_Dialog->objectName().isEmpty())
            Rotate_Dialog->setObjectName(QString::fromUtf8("Rotate_Dialog"));
        Rotate_Dialog->resize(276, 380);
        Rotate_Dialog->setMinimumSize(QSize(276, 380));
        Rotate_Dialog->setMaximumSize(QSize(276, 380));
        QFont font;
        font.setPointSize(10);
        Rotate_Dialog->setFont(font);
        _2 = new QPushButton(Rotate_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(184, 330, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Rotate_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(94, 330, 75, 23));
        groupBox = new QGroupBox(Rotate_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 9, 251, 121));
        groupBox->setFont(font);
        _99 = new QRadioButton(groupBox);
        _99->setObjectName(QString::fromUtf8("_99"));
        _99->setGeometry(QRect(20, 23, 181, 20));
        _99->setFont(font);
        _99->setLayoutDirection(Qt::RightToLeft);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 44, 181, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(20, 65, 181, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(20, 86, 181, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        label_7 = new QLabel(Rotate_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(29, 298, 101, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(Rotate_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(29, 270, 91, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(Rotate_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(12, 137, 251, 121));
        groupBox_2->setFont(font);
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(19, 82, 91, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(19, 26, 91, 20));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(19, 54, 91, 20));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _105 = new RCDoubleSpinBox(groupBox_2);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(126, 80, 99, 24));
        _105->setMinimum(-1000000000.000000000000000);
        _105->setMaximum(1000000000.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(126, 52, 99, 24));
        _104->setMinimum(-1000000000.000000000000000);
        _104->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(126, 24, 99, 24));
        _103->setMinimum(-1000000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _107 = new RCIntegerSpinBox(Rotate_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(138, 296, 99, 24));
        _107->setMaximum(10000);
        _106 = new RCDoubleSpinBox(Rotate_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(138, 268, 99, 24));
        _106->setMinimum(-100000.000000000000000);
        _106->setMaximum(100000.000000000000000);

        retranslateUi(Rotate_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Rotate_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Rotate_Dialog)
    {
        Rotate_Dialog->setWindowTitle(QCoreApplication::translate("Rotate_Dialog", "Rotate Options", nullptr));
        _2->setText(QCoreApplication::translate("Rotate_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Rotate_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Rotate_Dialog", "Define rotation by", nullptr));
        _99->setText(QCoreApplication::translate("Rotate_Dialog", "Single point", nullptr));
        _100->setText(QCoreApplication::translate("Rotate_Dialog", "Two points or a line", nullptr));
        _101->setText(QCoreApplication::translate("Rotate_Dialog", "Three points or an entity", nullptr));
        _102->setText(QCoreApplication::translate("Rotate_Dialog", "Two points and workplane", nullptr));
        label_7->setText(QCoreApplication::translate("Rotate_Dialog", "Number of copies", nullptr));
        label_8->setText(QCoreApplication::translate("Rotate_Dialog", "Angle", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Rotate_Dialog", "Translate by", nullptr));
        label_10->setText(QCoreApplication::translate("Rotate_Dialog", "Delta Z", nullptr));
        label_11->setText(QCoreApplication::translate("Rotate_Dialog", "Delta X", nullptr));
        label_12->setText(QCoreApplication::translate("Rotate_Dialog", "Delta Y", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rotate_Dialog: public Ui_Rotate_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROTATE_DIALOG_H
