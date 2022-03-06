/********************************************************************************
** Form generated from reading UI file 'PATCHMOVE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCHMOVE_DIALOG_H
#define UI_PATCHMOVE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_PatchMove_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QRadioButton *_102;
    QGroupBox *groupBox_2;
    QLabel *label_16;
    QLabel *label_20;
    QLabel *label_22;
    QLabel *label_21;
    QLabel *label_23;
    QLabel *label_17;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_107;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_108;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_109;
    QGroupBox *groupBox_3;
    QRadioButton *_111;
    QRadioButton *_110;
    QLabel *label_27;
    RCDoubleSpinBox *_112;
    QGroupBox *groupBox_4;
    QRadioButton *_114;
    QRadioButton *_113;
    QGroupBox *groupBox_5;
    QLabel *label_28;
    RCDoubleSpinBox *_115;
    QPushButton *_2;
    QPushButton *_1;
    QPushButton *_116;
    QPushButton *_117;
    QPushButton *_118;

    void setupUi(QDialog *PatchMove_Dialog)
    {
        if (PatchMove_Dialog->objectName().isEmpty())
            PatchMove_Dialog->setObjectName(QString::fromUtf8("PatchMove_Dialog"));
        PatchMove_Dialog->resize(360, 580);
        PatchMove_Dialog->setMinimumSize(QSize(360, 580));
        PatchMove_Dialog->setMaximumSize(QSize(360, 580));
        QFont font;
        font.setPointSize(10);
        PatchMove_Dialog->setFont(font);
        groupBox = new QGroupBox(PatchMove_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 331, 101));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(30, 23, 181, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(30, 46, 181, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(30, 70, 181, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        groupBox_2 = new QGroupBox(PatchMove_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 117, 331, 113));
        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(234, 27, 21, 20));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(149, 27, 21, 20));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(21, 26, 51, 20));
        label_22->setFont(font);
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(66, 27, 21, 20));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(149, 55, 21, 20));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(234, 55, 21, 20));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(21, 56, 51, 20));
        label_24->setFont(font);
        label_25 = new QLabel(groupBox_2);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(66, 56, 21, 20));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_26 = new QLabel(groupBox_2);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(100, 84, 101, 20));
        label_26->setFont(font);
        _103 = new RCDoubleSpinBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(90, 25, 50, 24));
        _103->setMinimum(-100000000.000000000000000);
        _103->setMaximum(100000000.000000000000000);
        _106 = new RCDoubleSpinBox(groupBox_2);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(90, 54, 51, 24));
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _106->setValue(1.000000000000000);
        _107 = new RCDoubleSpinBox(groupBox_2);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(174, 54, 51, 24));
        _107->setMinimum(-100000000.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _107->setValue(1.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(174, 25, 51, 24));
        _104->setMinimum(-100000000.000000000000000);
        _104->setMaximum(100000000.000000000000000);
        _108 = new RCDoubleSpinBox(groupBox_2);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(260, 54, 51, 24));
        _108->setMinimum(-100000000.000000000000000);
        _108->setMaximum(100000000.000000000000000);
        _108->setValue(1.000000000000000);
        _105 = new RCDoubleSpinBox(groupBox_2);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(260, 25, 51, 24));
        _105->setMinimum(-100000000.000000000000000);
        _105->setMaximum(100000000.000000000000000);
        _109 = new RCDoubleSpinBox(groupBox_2);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(194, 82, 117, 24));
        _109->setMaximum(360.000000000000000);
        groupBox_3 = new QGroupBox(PatchMove_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 234, 331, 101));
        _111 = new QRadioButton(groupBox_3);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(32, 44, 181, 20));
        _111->setFont(font);
        _111->setLayoutDirection(Qt::RightToLeft);
        _110 = new QRadioButton(groupBox_3);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(32, 21, 181, 20));
        _110->setFont(font);
        _110->setLayoutDirection(Qt::RightToLeft);
        label_27 = new QLabel(groupBox_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(182, 72, 21, 20));
        label_27->setFont(font);
        _112 = new RCDoubleSpinBox(groupBox_3);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(194, 70, 119, 24));
        _112->setMinimum(-100000.000000000000000);
        _112->setMaximum(100000000.000000000000000);
        groupBox_4 = new QGroupBox(PatchMove_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 341, 331, 81));
        _114 = new QRadioButton(groupBox_4);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(30, 50, 181, 20));
        _114->setFont(font);
        _114->setLayoutDirection(Qt::RightToLeft);
        _113 = new QRadioButton(groupBox_4);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(30, 26, 181, 20));
        _113->setFont(font);
        _113->setLayoutDirection(Qt::RightToLeft);
        groupBox_5 = new QGroupBox(PatchMove_Dialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 430, 331, 61));
        label_28 = new QLabel(groupBox_5);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(86, 26, 101, 20));
        label_28->setFont(font);
        _115 = new RCDoubleSpinBox(groupBox_5);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(190, 26, 119, 24));
        _115->setMinimum(-100000.000000000000000);
        _115->setMaximum(100000000.000000000000000);
        _2 = new QPushButton(PatchMove_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(266, 531, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PatchMove_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(176, 531, 75, 23));
        _1->setAutoDefault(true);
        _116 = new QPushButton(PatchMove_Dialog);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(10, 500, 121, 24));
        _116->setFont(font);
        _116->setAutoDefault(false);
        _117 = new QPushButton(PatchMove_Dialog);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(140, 500, 121, 24));
        _117->setFont(font);
        _117->setAutoDefault(false);
        _118 = new QPushButton(PatchMove_Dialog);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(270, 500, 71, 24));
        _118->setFont(font);
        _118->setAutoDefault(false);

        retranslateUi(PatchMove_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PatchMove_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PatchMove_Dialog)
    {
        PatchMove_Dialog->setWindowTitle(QCoreApplication::translate("PatchMove_Dialog", "Patch move control point Options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PatchMove_Dialog", "Define movement by", nullptr));
        _100->setText(QCoreApplication::translate("PatchMove_Dialog", "Coordinate entry", nullptr));
        _101->setText(QCoreApplication::translate("PatchMove_Dialog", "Two points", nullptr));
        _102->setText(QCoreApplication::translate("PatchMove_Dialog", "Flatten", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("PatchMove_Dialog", "Transformation Options", nullptr));
        label_16->setText(QCoreApplication::translate("PatchMove_Dialog", "Z", nullptr));
        label_20->setText(QCoreApplication::translate("PatchMove_Dialog", "Y", nullptr));
        label_22->setText(QCoreApplication::translate("PatchMove_Dialog", "Move", nullptr));
        label_21->setText(QCoreApplication::translate("PatchMove_Dialog", "X", nullptr));
        label_23->setText(QCoreApplication::translate("PatchMove_Dialog", "Y", nullptr));
        label_17->setText(QCoreApplication::translate("PatchMove_Dialog", "Z", nullptr));
        label_24->setText(QCoreApplication::translate("PatchMove_Dialog", "Scale", nullptr));
        label_25->setText(QCoreApplication::translate("PatchMove_Dialog", "X", nullptr));
        label_26->setText(QCoreApplication::translate("PatchMove_Dialog", "Rotation angle", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("PatchMove_Dialog", "Flatten Options", nullptr));
        _111->setText(QCoreApplication::translate("PatchMove_Dialog", "Flatten to plane entity ", nullptr));
        _110->setText(QCoreApplication::translate("PatchMove_Dialog", "Flatten to fixed Z ", nullptr));
        label_27->setText(QCoreApplication::translate("PatchMove_Dialog", "Z", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("PatchMove_Dialog", "Point Selection Options", nullptr));
        _114->setText(QCoreApplication::translate("PatchMove_Dialog", "Inside plane entity ", nullptr));
        _113->setText(QCoreApplication::translate("PatchMove_Dialog", "Single points", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("PatchMove_Dialog", "Smoothing Distance", nullptr));
        label_28->setText(QCoreApplication::translate("PatchMove_Dialog", "Smoothing distance", nullptr));
        _2->setText(QCoreApplication::translate("PatchMove_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PatchMove_Dialog", "OK", nullptr));
        _116->setText(QCoreApplication::translate("PatchMove_Dialog", "Delete Selected", nullptr));
        _117->setText(QCoreApplication::translate("PatchMove_Dialog", "Delete Unselected", nullptr));
        _118->setText(QCoreApplication::translate("PatchMove_Dialog", "Cut Out", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PatchMove_Dialog: public Ui_PatchMove_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCHMOVE_DIALOG_H
