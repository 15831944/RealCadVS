/********************************************************************************
** Form generated from reading UI file 'CHAMFER_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAMFER_DIALOG_H
#define UI_CHAMFER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Chamfer_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;

    void setupUi(QDialog *Chamfer_Dialog)
    {
        if (Chamfer_Dialog->objectName().isEmpty())
            Chamfer_Dialog->setObjectName(QString::fromUtf8("Chamfer_Dialog"));
        Chamfer_Dialog->resize(343, 260);
        Chamfer_Dialog->setMinimumSize(QSize(343, 260));
        Chamfer_Dialog->setMaximumSize(QSize(343, 260));
        QFont font;
        font.setPointSize(10);
        Chamfer_Dialog->setFont(font);
        _2 = new QPushButton(Chamfer_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(240, 210, 75, 23));
        _1 = new QPushButton(Chamfer_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(150, 210, 75, 23));
        groupBox = new QGroupBox(Chamfer_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 321, 91));
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(-30, 26, 231, 21));
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(-30, 56, 231, 21));
        _101->setLayoutDirection(Qt::RightToLeft);
        label_3 = new QLabel(Chamfer_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 112, 121, 16));
        label_4 = new QLabel(Chamfer_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 142, 203, 16));
        label_5 = new QLabel(Chamfer_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 170, 201, 16));
        _102 = new RCDoubleSpinBox(Chamfer_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(214, 110, 116, 22));
        _102->setDecimals(6);
        _102->setMaximum(100000000.000000000000000);
        _103 = new RCDoubleSpinBox(Chamfer_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(214, 140, 116, 22));
        _103->setDecimals(6);
        _103->setMaximum(100000000.000000000000000);
        _104 = new RCDoubleSpinBox(Chamfer_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(214, 170, 116, 22));
        _104->setDecimals(6);
        _104->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _101);
        QWidget::setTabOrder(_101, _100);

        retranslateUi(Chamfer_Dialog);

        QMetaObject::connectSlotsByName(Chamfer_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Chamfer_Dialog)
    {
        Chamfer_Dialog->setWindowTitle(QCoreApplication::translate("Chamfer_Dialog", "Insert chamfer options", nullptr));
        _2->setText(QCoreApplication::translate("Chamfer_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Chamfer_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Chamfer_Dialog", "Define chamfer by", nullptr));
        _100->setText(QCoreApplication::translate("Chamfer_Dialog", "Length of chamfer", nullptr));
        _101->setText(QCoreApplication::translate("Chamfer_Dialog", "Distance from intersection", nullptr));
        label_3->setText(QCoreApplication::translate("Chamfer_Dialog", "Chamfer length", nullptr));
        label_4->setText(QCoreApplication::translate("Chamfer_Dialog", "Chamfer distance along line 1", nullptr));
        label_5->setText(QCoreApplication::translate("Chamfer_Dialog", "Chamfer distance along line 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chamfer_Dialog: public Ui_Chamfer_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAMFER_DIALOG_H
