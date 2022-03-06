/********************************************************************************
** Form generated from reading UI file 'COPYALONG_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYALONG_DIALOG_H
#define UI_COPYALONG_DIALOG_H

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

class Ui_CopyAlong_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QRadioButton *_100;
    QRadioButton *_101;
    QGroupBox *groupBox;
    QRadioButton *_102;
    QRadioButton *_103;
    QLabel *label_4;
    QLabel *label_3;
    RCDoubleSpinBox *_104;
    RCIntegerSpinBox *_105;

    void setupUi(QDialog *CopyAlong_Dialog)
    {
        if (CopyAlong_Dialog->objectName().isEmpty())
            CopyAlong_Dialog->setObjectName(QString::fromUtf8("CopyAlong_Dialog"));
        CopyAlong_Dialog->resize(280, 270);
        CopyAlong_Dialog->setMinimumSize(QSize(252, 270));
        CopyAlong_Dialog->setMaximumSize(QSize(287, 270));
        QFont font;
        font.setPointSize(10);
        CopyAlong_Dialog->setFont(font);
        _2 = new QPushButton(CopyAlong_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(196, 220, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CopyAlong_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(106, 220, 75, 23));
        _100 = new QRadioButton(CopyAlong_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(54, 10, 137, 20));
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(CopyAlong_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(54, 32, 137, 20));
        _101->setLayoutDirection(Qt::RightToLeft);
        groupBox = new QGroupBox(CopyAlong_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 70, 261, 141));
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(-30, 20, 211, 20));
        _102->setLayoutDirection(Qt::RightToLeft);
        _103 = new QRadioButton(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(-30, 40, 211, 20));
        _103->setLayoutDirection(Qt::RightToLeft);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 100, 151, 21));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 70, 117, 21));
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(158, 68, 87, 24));
        _104->setMaximum(100000000.000000000000000);
        _105 = new RCIntegerSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(158, 98, 87, 24));
        _105->setMaximum(10000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _103);
        QWidget::setTabOrder(_103, _102);

        retranslateUi(CopyAlong_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CopyAlong_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CopyAlong_Dialog)
    {
        CopyAlong_Dialog->setWindowTitle(QCoreApplication::translate("CopyAlong_Dialog", "Copy along options", nullptr));
        _2->setText(QCoreApplication::translate("CopyAlong_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CopyAlong_Dialog", "OK", nullptr));
        _100->setText(QCoreApplication::translate("CopyAlong_Dialog", "Original orientation", nullptr));
        _101->setText(QCoreApplication::translate("CopyAlong_Dialog", "Parallel to entity", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CopyAlong_Dialog", "Spacing options", nullptr));
        _102->setText(QCoreApplication::translate("CopyAlong_Dialog", " By Spacing ", nullptr));
        _103->setText(QCoreApplication::translate("CopyAlong_Dialog", "By quantity", nullptr));
        label_4->setText(QCoreApplication::translate("CopyAlong_Dialog", "Number of copies", nullptr));
        label_3->setText(QCoreApplication::translate("CopyAlong_Dialog", "Spacing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CopyAlong_Dialog: public Ui_CopyAlong_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYALONG_DIALOG_H
