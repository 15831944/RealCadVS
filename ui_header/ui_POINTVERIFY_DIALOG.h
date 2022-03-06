/********************************************************************************
** Form generated from reading UI file 'POINTVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTVERIFY_DIALOG_H
#define UI_POINTVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_PointVerify_Dialog
{
public:
    QLabel *label_5;
    QLineEdit *_104;
    QPushButton *_1;
    QGroupBox *groupBox;
    QPushButton *_1002;
    QLabel *label_103;
    QPushButton *_1001;
    QSpinBox *_102;
    QLabel *label_100;
    QLabel *label_102;
    QPushButton *_1000;
    QSpinBox *_100;
    WeightSpinBox *_103;
    LayerSpinBox *_101;
    QPushButton *_1003;
    QLabel *label_101;
    QPushButton *_2;

    void setupUi(QDialog *PointVerify_Dialog)
    {
        if (PointVerify_Dialog->objectName().isEmpty())
            PointVerify_Dialog->setObjectName(QString::fromUtf8("PointVerify_Dialog"));
        PointVerify_Dialog->resize(413, 169);
        PointVerify_Dialog->setMinimumSize(QSize(413, 169));
        PointVerify_Dialog->setMaximumSize(QSize(413, 169));
        QFont font;
        font.setPointSize(10);
        PointVerify_Dialog->setFont(font);
        label_5 = new QLabel(PointVerify_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(24, 102, 61, 20));
        label_5->setFont(font);
        _104 = new QLineEdit(PointVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(92, 103, 287, 20));
        _104->setFont(font);
        _104->setReadOnly(true);
        _1 = new QPushButton(PointVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(222, 132, 75, 24));
        _1->setFont(font);
        groupBox = new QGroupBox(PointVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 2, 381, 87));
        _1002 = new QPushButton(groupBox);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(124, 55, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(158, 56, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(groupBox);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(346, 28, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(46, 56, 77, 22));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(10, 31, 39, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(10, 57, 41, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(groupBox);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(124, 29, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(46, 30, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(196, 54, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(196, 28, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(true);
        _1003->setGeometry(QRect(274, 54, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(158, 30, 33, 20));
        label_101->setFont(font);
        _2 = new QPushButton(PointVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(310, 132, 75, 24));
        _2->setFont(font);
        _2->setAutoDefault(false);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _104);
        QWidget::setTabOrder(_104, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(PointVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PointVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PointVerify_Dialog)
    {
        PointVerify_Dialog->setWindowTitle(QCoreApplication::translate("PointVerify_Dialog", "Point Verify", nullptr));
        label_5->setText(QCoreApplication::translate("PointVerify_Dialog", "Position", nullptr));
        _1->setText(QCoreApplication::translate("PointVerify_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PointVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("PointVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("PointVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("PointVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("PointVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("PointVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("PointVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("PointVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("PointVerify_Dialog", "Layer", nullptr));
        _2->setText(QCoreApplication::translate("PointVerify_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PointVerify_Dialog: public Ui_PointVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTVERIFY_DIALOG_H
