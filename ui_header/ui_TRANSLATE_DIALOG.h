/********************************************************************************
** Form generated from reading UI file 'TRANSLATE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSLATE_DIALOG_H
#define UI_TRANSLATE_DIALOG_H

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

class Ui_Translate_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_7;
    QGroupBox *groupBox_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QRadioButton *_101;
    QRadioButton *_100;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;
    RCIntegerSpinBox *_105;

    void setupUi(QDialog *Translate_Dialog)
    {
        if (Translate_Dialog->objectName().isEmpty())
            Translate_Dialog->setObjectName(QString::fromUtf8("Translate_Dialog"));
        Translate_Dialog->resize(302, 270);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Translate_Dialog->sizePolicy().hasHeightForWidth());
        Translate_Dialog->setSizePolicy(sizePolicy);
        Translate_Dialog->setMinimumSize(QSize(302, 270));
        Translate_Dialog->setMaximumSize(QSize(302, 270));
        _2 = new QPushButton(Translate_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(212, 216, 75, 27));
        QFont font;
        font.setPointSize(10);
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Translate_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(122, 216, 75, 28));
        _1->setFont(font);
        label_7 = new QLabel(Translate_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(16, 182, 151, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(Translate_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 12, 281, 151));
        groupBox_3->setFont(font);
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(8, 65, 151, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(8, 93, 151, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(8, 120, 151, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QRadioButton(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(30, 40, 161, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _100 = new QRadioButton(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(30, 20, 161, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _102 = new RCDoubleSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(164, 64, 101, 24));
        _102->setMinimum(-1000000000.000000000000000);
        _102->setMaximum(1000000000.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(164, 92, 101, 24));
        _103->setMinimum(-1000000000.000000000000000);
        _103->setMaximum(1000000000.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox_3);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(164, 119, 101, 24));
        _104->setMinimum(-1000000000.000000000000000);
        _104->setMaximum(1000000000.000000000000000);
        _105 = new RCIntegerSpinBox(Translate_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(174, 180, 101, 24));
        _105->setMinimum(1);
        _105->setMaximum(10000);

        retranslateUi(Translate_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Translate_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Translate_Dialog)
    {
        Translate_Dialog->setWindowTitle(QCoreApplication::translate("Translate_Dialog", "Translate (Copy) Options", nullptr));
        _2->setText(QCoreApplication::translate("Translate_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Translate_Dialog", "OK", nullptr));
        label_7->setText(QCoreApplication::translate("Translate_Dialog", "Number of copies", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Translate_Dialog", "Define Translation by", nullptr));
        label_4->setText(QCoreApplication::translate("Translate_Dialog", "Delta X", nullptr));
        label_5->setText(QCoreApplication::translate("Translate_Dialog", "Delta Y", nullptr));
        label_6->setText(QCoreApplication::translate("Translate_Dialog", "Delta Z", nullptr));
        _101->setText(QCoreApplication::translate("Translate_Dialog", "Two points", nullptr));
        _100->setText(QCoreApplication::translate("Translate_Dialog", "Coordinate entry", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Translate_Dialog: public Ui_Translate_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSLATE_DIALOG_H
