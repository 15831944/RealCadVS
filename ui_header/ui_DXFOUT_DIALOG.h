/********************************************************************************
** Form generated from reading UI file 'DXFOUT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DXFOUT_DIALOG_H
#define UI_DXFOUT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_DXFOut_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox_3;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_3;
    QLabel *label_4;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;

    void setupUi(QDialog *DXFOut_Dialog)
    {
        if (DXFOut_Dialog->objectName().isEmpty())
            DXFOut_Dialog->setObjectName(QString::fromUtf8("DXFOut_Dialog"));
        DXFOut_Dialog->resize(321, 190);
        DXFOut_Dialog->setMinimumSize(QSize(321, 190));
        DXFOut_Dialog->setMaximumSize(QSize(321, 190));
        QFont font;
        font.setPointSize(10);
        DXFOut_Dialog->setFont(font);
        _2 = new QPushButton(DXFOut_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(230, 154, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(DXFOut_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(130, 154, 75, 23));
        groupBox_3 = new QGroupBox(DXFOut_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 12, 281, 71));
        _100 = new QRadioButton(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(32, 32, 101, 17));
        _100->setLayoutDirection(Qt::LeftToRight);
        _101 = new QRadioButton(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(168, 32, 101, 17));
        _101->setLayoutDirection(Qt::LeftToRight);
        label_3 = new QLabel(DXFOut_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 96, 141, 20));
        label_4 = new QLabel(DXFOut_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(28, 122, 139, 20));
        _102 = new RCDoubleSpinBox(DXFOut_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(190, 94, 100, 24));
        _103 = new RCDoubleSpinBox(DXFOut_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(190, 122, 100, 24));

        retranslateUi(DXFOut_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DXFOut_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DXFOut_Dialog)
    {
        DXFOut_Dialog->setWindowTitle(QCoreApplication::translate("DXFOut_Dialog", "DXF export options", nullptr));
        _2->setText(QCoreApplication::translate("DXFOut_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DXFOut_Dialog", "OK", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("DXFOut_Dialog", "DXF file units", nullptr));
        _100->setText(QCoreApplication::translate("DXFOut_Dialog", "mm", nullptr));
        _101->setText(QCoreApplication::translate("DXFOut_Dialog", "inches", nullptr));
        label_3->setText(QCoreApplication::translate("DXFOut_Dialog", "Curve segment length", nullptr));
        label_4->setText(QCoreApplication::translate("DXFOut_Dialog", "Scale text by", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DXFOut_Dialog: public Ui_DXFOut_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DXFOUT_DIALOG_H
