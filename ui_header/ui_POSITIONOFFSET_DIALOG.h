/********************************************************************************
** Form generated from reading UI file 'POSITIONOFFSET_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSITIONOFFSET_DIALOG_H
#define UI_POSITIONOFFSET_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_PositionOffset_Dialog
{
public:
    QGroupBox *groupBox;
    QCheckBox *_100;
    QCheckBox *_102;
    QLabel *label_9;
    QLabel *label_8;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_103;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *PositionOffset_Dialog)
    {
        if (PositionOffset_Dialog->objectName().isEmpty())
            PositionOffset_Dialog->setObjectName(QString::fromUtf8("PositionOffset_Dialog"));
        PositionOffset_Dialog->resize(463, 140);
        PositionOffset_Dialog->setMinimumSize(QSize(463, 140));
        PositionOffset_Dialog->setMaximumSize(QSize(463, 140));
        QFont font;
        font.setPointSize(10);
        PositionOffset_Dialog->setFont(font);
        groupBox = new QGroupBox(PositionOffset_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 443, 81));
        groupBox->setFont(font);
        _100 = new QCheckBox(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(10, 24, 141, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _100->setAutoFillBackground(false);
        _100->setCheckable(true);
        _100->setChecked(false);
        _100->setTristate(false);
        _102 = new QCheckBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(10, 48, 141, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setAutoFillBackground(false);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(180, 48, 121, 20));
        label_9->setFont(font);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(180, 24, 121, 20));
        label_8->setFont(font);
        _101 = new RCDoubleSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(282, 21, 100, 24));
        _101->setMinimum(-100000000.000000000000000);
        _101->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(282, 46, 100, 24));
        _103->setMinimum(-100000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _2 = new QPushButton(PositionOffset_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(374, 94, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PositionOffset_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(282, 94, 75, 23));
        _1->setAutoDefault(false);

        retranslateUi(PositionOffset_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PositionOffset_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PositionOffset_Dialog)
    {
        PositionOffset_Dialog->setWindowTitle(QCoreApplication::translate("PositionOffset_Dialog", "Position offset from entities", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PositionOffset_Dialog", "Position by", nullptr));
        _100->setText(QCoreApplication::translate("PositionOffset_Dialog", "Fixed distance 1", nullptr));
        _102->setText(QCoreApplication::translate("PositionOffset_Dialog", "Fixed distance 2", nullptr));
        label_9->setText(QCoreApplication::translate("PositionOffset_Dialog", "Offset distance 2", nullptr));
        label_8->setText(QCoreApplication::translate("PositionOffset_Dialog", "Offset distance 1", nullptr));
        _2->setText(QCoreApplication::translate("PositionOffset_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PositionOffset_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PositionOffset_Dialog: public Ui_PositionOffset_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSITIONOFFSET_DIALOG_H
