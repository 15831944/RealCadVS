/********************************************************************************
** Form generated from reading UI file 'TRIM_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIM_DIALOG_H
#define UI_TRIM_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Trim_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_101;
    QRadioButton *_102;
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label;
    RCDoubleSpinBox *_100;

    void setupUi(QDialog *Trim_Dialog)
    {
        if (Trim_Dialog->objectName().isEmpty())
            Trim_Dialog->setObjectName(QString::fromUtf8("Trim_Dialog"));
        Trim_Dialog->resize(210, 191);
        Trim_Dialog->setMinimumSize(QSize(210, 191));
        Trim_Dialog->setMaximumSize(QSize(210, 191));
        groupBox = new QGroupBox(Trim_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 60, 191, 91));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        groupBox->setFont(font);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(14, 27, 141, 20));
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(16, 51, 139, 20));
        _102->setLayoutDirection(Qt::RightToLeft);
        _2 = new QPushButton(Trim_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(116, 156, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Trim_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(26, 156, 75, 23));
        label = new QLabel(Trim_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 91, 20));
        QFont font1;
        font1.setPointSize(10);
        label->setFont(font1);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new RCDoubleSpinBox(Trim_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(96, 17, 100, 24));
        _100->setFont(font);
        _100->setMinimum(-100000000.000000000000000);
        _100->setMaximum(100000000.000000000000000);

        retranslateUi(Trim_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Trim_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Trim_Dialog)
    {
        Trim_Dialog->setWindowTitle(QCoreApplication::translate("Trim_Dialog", "Trim to length", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Trim_Dialog", "Trim entity", nullptr));
        _101->setText(QCoreApplication::translate("Trim_Dialog", "By selected length", nullptr));
        _102->setText(QCoreApplication::translate("Trim_Dialog", "To selected length", nullptr));
        _2->setText(QCoreApplication::translate("Trim_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Trim_Dialog", "OK", nullptr));
        label->setText(QCoreApplication::translate("Trim_Dialog", "Trim length", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Trim_Dialog: public Ui_Trim_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIM_DIALOG_H
