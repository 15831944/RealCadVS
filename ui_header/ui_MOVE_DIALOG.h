/********************************************************************************
** Form generated from reading UI file 'MOVE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOVE_DIALOG_H
#define UI_MOVE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Move_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;

    void setupUi(QDialog *Move_Dialog)
    {
        if (Move_Dialog->objectName().isEmpty())
            Move_Dialog->setObjectName(QString::fromUtf8("Move_Dialog"));
        Move_Dialog->resize(315, 240);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Move_Dialog->sizePolicy().hasHeightForWidth());
        Move_Dialog->setSizePolicy(sizePolicy);
        Move_Dialog->setMinimumSize(QSize(315, 240));
        Move_Dialog->setMaximumSize(QSize(16777215, 240));
        QFont font;
        font.setPointSize(10);
        Move_Dialog->setFont(font);
        _2 = new QPushButton(Move_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(230, 192, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Move_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(140, 192, 75, 23));
        groupBox = new QGroupBox(Move_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 9, 291, 171));
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
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(80, 103, 91, 20));
        label_13->setFont(font);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(80, 75, 91, 20));
        label_14->setFont(font);
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(80, 130, 91, 20));
        label_15->setFont(font);
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _102 = new RCDoubleSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(168, 72, 101, 24));
        _102->setMinimum(-1000000000.000000000000000);
        _102->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(168, 100, 101, 24));
        _103->setMinimum(-1000000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(168, 128, 101, 24));
        _104->setMinimum(-1000000000.000000000000000);
        _104->setMaximum(1000000000.000000000000000);

        retranslateUi(Move_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Move_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Move_Dialog)
    {
        Move_Dialog->setWindowTitle(QCoreApplication::translate("Move_Dialog", "Move Options", nullptr));
        _2->setText(QCoreApplication::translate("Move_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Move_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Move_Dialog", "Define translation by", nullptr));
        _100->setText(QCoreApplication::translate("Move_Dialog", "Coordinate entry", nullptr));
        _101->setText(QCoreApplication::translate("Move_Dialog", "Two points", nullptr));
        label_13->setText(QCoreApplication::translate("Move_Dialog", "Delta Y", nullptr));
        label_14->setText(QCoreApplication::translate("Move_Dialog", "Delta X", nullptr));
        label_15->setText(QCoreApplication::translate("Move_Dialog", "Delta Z", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Move_Dialog: public Ui_Move_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOVE_DIALOG_H
