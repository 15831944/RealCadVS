/********************************************************************************
** Form generated from reading UI file 'STRETCH_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STRETCH_DIALOG_H
#define UI_STRETCH_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Stretch_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QGroupBox *groupBox_2;
    QRadioButton *_105;
    QRadioButton *_106;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;

    void setupUi(QDialog *Stretch_Dialog)
    {
        if (Stretch_Dialog->objectName().isEmpty())
            Stretch_Dialog->setObjectName(QString::fromUtf8("Stretch_Dialog"));
        Stretch_Dialog->resize(280, 320);
        Stretch_Dialog->setMinimumSize(QSize(280, 320));
        Stretch_Dialog->setMaximumSize(QSize(280, 320));
        QFont font;
        font.setPointSize(10);
        Stretch_Dialog->setFont(font);
        _2 = new QPushButton(Stretch_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(190, 270, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Stretch_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(106, 270, 75, 23));
        groupBox = new QGroupBox(Stretch_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 9, 251, 81));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(18, 23, 167, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(28, 44, 157, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        label_7 = new QLabel(Stretch_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 213, 71, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(Stretch_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 185, 71, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9 = new QLabel(Stretch_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(40, 240, 71, 20));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(Stretch_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(13, 98, 251, 81));
        groupBox_2->setFont(font);
        _105 = new QRadioButton(groupBox_2);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(18, 23, 165, 20));
        _105->setFont(font);
        _105->setLayoutDirection(Qt::RightToLeft);
        _106 = new QRadioButton(groupBox_2);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(16, 44, 167, 20));
        _106->setFont(font);
        _106->setLayoutDirection(Qt::RightToLeft);
        _102 = new RCDoubleSpinBox(Stretch_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(130, 182, 101, 24));
        _102->setMinimum(-1000000000.000000000000000);
        _102->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(Stretch_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(130, 210, 101, 24));
        _103->setMinimum(-1000000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _104 = new RCDoubleSpinBox(Stretch_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(130, 238, 101, 24));
        _104->setDecimals(2);
        _104->setMinimum(-1000000000.000000000000000);
        _104->setMaximum(1000000000.000000000000000);

        retranslateUi(Stretch_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Stretch_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Stretch_Dialog)
    {
        Stretch_Dialog->setWindowTitle(QCoreApplication::translate("Stretch_Dialog", "Stretch Options", nullptr));
        _2->setText(QCoreApplication::translate("Stretch_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Stretch_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Stretch_Dialog", "Define stretch by", nullptr));
        _100->setText(QCoreApplication::translate("Stretch_Dialog", "Coordinate entry        ", nullptr));
        _101->setText(QCoreApplication::translate("Stretch_Dialog", "Two points                 ", nullptr));
        label_7->setText(QCoreApplication::translate("Stretch_Dialog", "Delta Y", nullptr));
        label_8->setText(QCoreApplication::translate("Stretch_Dialog", "Delta X", nullptr));
        label_9->setText(QCoreApplication::translate("Stretch_Dialog", "Delta Z", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Stretch_Dialog", "Define entities to stretch by", nullptr));
        _105->setText(QCoreApplication::translate("Stretch_Dialog", "Current selection      ", nullptr));
        _106->setText(QCoreApplication::translate("Stretch_Dialog", "Window                    ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Stretch_Dialog: public Ui_Stretch_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STRETCH_DIALOG_H
