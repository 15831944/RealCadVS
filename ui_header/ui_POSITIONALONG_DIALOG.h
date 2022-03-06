/********************************************************************************
** Form generated from reading UI file 'POSITIONALONG_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSITIONALONG_DIALOG_H
#define UI_POSITIONALONG_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_PositionAlong_Dialog
{
public:
    QGroupBox *groupBox;
    QLabel *label_9;
    QLabel *label_8;
    QRadioButton *_200;
    QRadioButton *_201;
    QLabel *label_6;
    QComboBox *_102;
    QRadioButton *_202;
    RCDoubleSpinBox *_100;
    RCDoubleSpinBox *_101;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *PositionAlong_Dialog)
    {
        if (PositionAlong_Dialog->objectName().isEmpty())
            PositionAlong_Dialog->setObjectName(QString::fromUtf8("PositionAlong_Dialog"));
        PositionAlong_Dialog->resize(311, 170);
        PositionAlong_Dialog->setMinimumSize(QSize(311, 170));
        PositionAlong_Dialog->setMaximumSize(QSize(311, 170));
        QFont font;
        font.setPointSize(10);
        PositionAlong_Dialog->setFont(font);
        groupBox = new QGroupBox(PositionAlong_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 291, 108));
        groupBox->setFont(font);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(40, 50, 101, 20));
        label_9->setFont(font);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 24, 101, 20));
        label_8->setFont(font);
        _200 = new QRadioButton(groupBox);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setGeometry(QRect(228, 24, 21, 21));
        _200->setLayoutDirection(Qt::LeftToRight);
        _201 = new QRadioButton(groupBox);
        _201->setObjectName(QString::fromUtf8("_201"));
        _201->setGeometry(QRect(228, 48, 21, 21));
        _201->setLayoutDirection(Qt::LeftToRight);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 74, 111, 20));
        label_6->setFont(font);
        _102 = new QComboBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(116, 72, 100, 22));
        _202 = new QRadioButton(groupBox);
        _202->setObjectName(QString::fromUtf8("_202"));
        _202->setGeometry(QRect(228, 72, 21, 21));
        _202->setLayoutDirection(Qt::LeftToRight);
        _100 = new RCDoubleSpinBox(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(119, 22, 100, 24));
        _100->setMinimum(-1000000.000000000000000);
        _100->setMaximum(1000000.000000000000000);
        _101 = new RCDoubleSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(118, 48, 100, 24));
        _2 = new QPushButton(PositionAlong_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(224, 122, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(PositionAlong_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(134, 122, 75, 23));
        _1->setAutoDefault(false);

        retranslateUi(PositionAlong_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PositionAlong_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PositionAlong_Dialog)
    {
        PositionAlong_Dialog->setWindowTitle(QCoreApplication::translate("PositionAlong_Dialog", "Position along an entity by", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PositionAlong_Dialog", "Position by", nullptr));
        label_9->setText(QCoreApplication::translate("PositionAlong_Dialog", "Percentage", nullptr));
        label_8->setText(QCoreApplication::translate("PositionAlong_Dialog", "Distance", nullptr));
        _200->setText(QString());
        _201->setText(QString());
        label_6->setText(QCoreApplication::translate("PositionAlong_Dialog", "Fraction", nullptr));
        _202->setText(QString());
        _2->setText(QCoreApplication::translate("PositionAlong_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("PositionAlong_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PositionAlong_Dialog: public Ui_PositionAlong_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSITIONALONG_DIALOG_H
