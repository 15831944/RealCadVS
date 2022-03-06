/********************************************************************************
** Form generated from reading UI file 'MEASURELENGTHAREA_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASURELENGTHAREA_DIALOG_H
#define UI_MEASURELENGTHAREA_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_MeasureLengthArea_Dialog
{
public:
    QCheckBox *_110;
    QLabel *label_4;
    QGroupBox *groupBox;
    QLineEdit *_100;
    QLabel *label_5;
    QLineEdit *_103;
    QLabel *label_6;
    QLineEdit *_101;
    QLineEdit *_104;
    QLineEdit *_105;
    QLineEdit *_102;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *_106;
    RCDoubleSpinBox *_114;

    void setupUi(QDialog *MeasureLengthArea_Dialog)
    {
        if (MeasureLengthArea_Dialog->objectName().isEmpty())
            MeasureLengthArea_Dialog->setObjectName(QString::fromUtf8("MeasureLengthArea_Dialog"));
        MeasureLengthArea_Dialog->resize(389, 214);
        MeasureLengthArea_Dialog->setMinimumSize(QSize(389, 214));
        MeasureLengthArea_Dialog->setMaximumSize(QSize(389, 214));
        QFont font;
        font.setPointSize(10);
        MeasureLengthArea_Dialog->setFont(font);
        _110 = new QCheckBox(MeasureLengthArea_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(60, 12, 131, 20));
        _110->setLayoutDirection(Qt::LeftToRight);
        label_4 = new QLabel(MeasureLengthArea_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(212, 13, 53, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox = new QGroupBox(MeasureLengthArea_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 371, 151));
        _100 = new QLineEdit(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(85, 30, 131, 20));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(15, 30, 71, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _103 = new QLineEdit(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(225, 30, 131, 20));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(15, 57, 71, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QLineEdit(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(85, 57, 131, 20));
        _104 = new QLineEdit(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(225, 57, 131, 20));
        _105 = new QLineEdit(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(225, 84, 131, 20));
        _102 = new QLineEdit(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(85, 84, 131, 20));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(15, 84, 71, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(15, 110, 81, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QLineEdit(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(103, 110, 253, 20));
        _114 = new RCDoubleSpinBox(MeasureLengthArea_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(266, 10, 100, 24));
        _114->setAccelerated(true);
        _114->setMinimum(-100000000.000000000000000);
        _114->setMaximum(100000000.000000000000000);

        retranslateUi(MeasureLengthArea_Dialog);

        QMetaObject::connectSlotsByName(MeasureLengthArea_Dialog);
    } // setupUi

    void retranslateUi(QDialog *MeasureLengthArea_Dialog)
    {
        MeasureLengthArea_Dialog->setWindowTitle(QCoreApplication::translate("MeasureLengthArea_Dialog", "Measure Length and Area", nullptr));
        _110->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Enable scale", nullptr));
        label_4->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Scale", nullptr));
        groupBox->setTitle(QString());
        label_5->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Number", nullptr));
        label_6->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Last", nullptr));
        label_7->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Total", nullptr));
        label_8->setText(QCoreApplication::translate("MeasureLengthArea_Dialog", "Last direction", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureLengthArea_Dialog: public Ui_MeasureLengthArea_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASURELENGTHAREA_DIALOG_H
