/********************************************************************************
** Form generated from reading UI file 'DXFIN_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DXFIN_DIALOG_H
#define UI_DXFIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_DXFIn_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QCheckBox *_102;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *DXFIn_Dialog)
    {
        if (DXFIn_Dialog->objectName().isEmpty())
            DXFIn_Dialog->setObjectName(QString::fromUtf8("DXFIn_Dialog"));
        DXFIn_Dialog->resize(321, 162);
        DXFIn_Dialog->setMinimumSize(QSize(321, 162));
        DXFIn_Dialog->setMaximumSize(QSize(321, 162));
        QFont font;
        font.setPointSize(10);
        DXFIn_Dialog->setFont(font);
        groupBox = new QGroupBox(DXFIn_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 9, 281, 71));
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(32, 32, 101, 17));
        _100->setLayoutDirection(Qt::LeftToRight);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(168, 32, 101, 17));
        _101->setLayoutDirection(Qt::LeftToRight);
        _102 = new QCheckBox(DXFIn_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(30, 90, 221, 21));
        _102->setLayoutDirection(Qt::RightToLeft);
        _2 = new QPushButton(DXFIn_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(232, 128, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(DXFIn_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(140, 130, 75, 23));
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(DXFIn_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DXFIn_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DXFIn_Dialog)
    {
        DXFIn_Dialog->setWindowTitle(QCoreApplication::translate("DXFIn_Dialog", "DXF import options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DXFIn_Dialog", "DXF file units", nullptr));
        _100->setText(QCoreApplication::translate("DXFIn_Dialog", "mm", nullptr));
        _101->setText(QCoreApplication::translate("DXFIn_Dialog", "inches", nullptr));
        _102->setText(QCoreApplication::translate("DXFIn_Dialog", "Clear existing layer names", nullptr));
        _2->setText(QCoreApplication::translate("DXFIn_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DXFIn_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DXFIn_Dialog: public Ui_DXFIn_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DXFIN_DIALOG_H
