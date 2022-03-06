/********************************************************************************
** Form generated from reading UI file 'SCALE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCALE_DIALOG_H
#define UI_SCALE_DIALOG_H

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

class Ui_Scale_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QPushButton *_107;
    QPushButton *_108;
    QPushButton *_109;
    QLabel *label_11;
    QLabel *label_12;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_104;
    QLabel *label_13;
    RCIntegerSpinBox *_106;

    void setupUi(QDialog *Scale_Dialog)
    {
        if (Scale_Dialog->objectName().isEmpty())
            Scale_Dialog->setObjectName(QString::fromUtf8("Scale_Dialog"));
        Scale_Dialog->resize(380, 320);
        Scale_Dialog->setMinimumSize(QSize(380, 320));
        Scale_Dialog->setMaximumSize(QSize(380, 320));
        QFont font;
        font.setPointSize(10);
        Scale_Dialog->setFont(font);
        _2 = new QPushButton(Scale_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(270, 270, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Scale_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(182, 270, 75, 23));
        groupBox = new QGroupBox(Scale_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 9, 355, 211));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(30, 23, 181, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(30, 46, 181, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(23, 80, 111, 20));
        label_8->setFont(font);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(23, 110, 111, 20));
        label_9->setFont(font);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(23, 140, 111, 20));
        label_10->setFont(font);
        _107 = new QPushButton(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(220, 108, 41, 23));
        _108 = new QPushButton(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(220, 140, 41, 23));
        _109 = new QPushButton(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(290, 140, 41, 23));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(220, 80, 101, 20));
        label_11->setFont(font);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(23, 170, 111, 20));
        label_12->setFont(font);
        _105 = new RCDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(130, 168, 83, 24));
        _105->setMinimum(0.000000000000000);
        _105->setMaximum(1000000000.000000000000000);
        _105->setValue(100.000000000000000);
        _102 = new RCDoubleSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(130, 78, 83, 24));
        _102->setMinimum(-1000000000.000000000000000);
        _102->setMaximum(100000000.000000000000000);
        _102->setValue(1.000000000000000);
        _103 = new RCDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(130, 108, 83, 24));
        _103->setMinimum(-100000000.000000000000000);
        _103->setMaximum(100000000.000000000000000);
        _103->setValue(1.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(130, 138, 83, 24));
        _104->setMinimum(-100000000.000000000000000);
        _104->setMaximum(100000000.000000000000000);
        _104->setValue(1.000000000000000);
        label_13 = new QLabel(Scale_Dialog);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(35, 230, 111, 20));
        label_13->setFont(font);
        _106 = new RCIntegerSpinBox(Scale_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(144, 228, 83, 24));
        _106->setMinimum(0);
        _106->setMaximum(100000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Scale_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Scale_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Scale_Dialog)
    {
        Scale_Dialog->setWindowTitle(QCoreApplication::translate("Scale_Dialog", "Scale Options", nullptr));
        _2->setText(QCoreApplication::translate("Scale_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Scale_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Scale_Dialog", "Define scale factors by", nullptr));
        _100->setText(QCoreApplication::translate("Scale_Dialog", "Direct entry", nullptr));
        _101->setText(QCoreApplication::translate("Scale_Dialog", "Two points and length", nullptr));
        label_8->setText(QCoreApplication::translate("Scale_Dialog", "X scale", nullptr));
        label_9->setText(QCoreApplication::translate("Scale_Dialog", "Y scale", nullptr));
        label_10->setText(QCoreApplication::translate("Scale_Dialog", "Z scale", nullptr));
        _107->setText(QCoreApplication::translate("Scale_Dialog", "X", nullptr));
        _108->setText(QCoreApplication::translate("Scale_Dialog", "X", nullptr));
        _109->setText(QCoreApplication::translate("Scale_Dialog", "Y", nullptr));
        label_11->setText(QCoreApplication::translate("Scale_Dialog", "Copy From:", nullptr));
        label_12->setText(QCoreApplication::translate("Scale_Dialog", "Length", nullptr));
        label_13->setText(QCoreApplication::translate("Scale_Dialog", "Number of copies", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Scale_Dialog: public Ui_Scale_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCALE_DIALOG_H
