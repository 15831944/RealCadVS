/********************************************************************************
** Form generated from reading UI file 'LIGHTVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTVERIFY_DIALOG_H
#define UI_LIGHTVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_LightVerify_Dialog
{
public:
    QLabel *label_3;
    QCheckBox *_105;
    QCheckBox *_106;
    QLabel *label_4;
    QGroupBox *groupBox;
    QCheckBox *_108;
    QLabel *label_5;
    QLabel *label_6;
    QDoubleSpinBox *_109;
    QDoubleSpinBox *_110;
    QGroupBox *groupBox_2;
    QCheckBox *_111;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *_112;
    QDoubleSpinBox *_113;
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *Defaults;
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
    QDoubleSpinBox *_104;
    QDoubleSpinBox *_107;
    QGroupBox *_2000;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *Date;
    QLabel *Latitude;
    QLabel *Time;
    QLabel *Longitude;
    QLabel *TimeZone;

    void setupUi(QDialog *LightVerify_Dialog)
    {
        if (LightVerify_Dialog->objectName().isEmpty())
            LightVerify_Dialog->setObjectName(QString::fromUtf8("LightVerify_Dialog"));
        LightVerify_Dialog->resize(429, 446);
        LightVerify_Dialog->setMinimumSize(QSize(429, 446));
        LightVerify_Dialog->setMaximumSize(QSize(429, 446));
        QFont font;
        font.setPointSize(10);
        LightVerify_Dialog->setFont(font);
        label_3 = new QLabel(LightVerify_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(102, 208, 97, 21));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _105 = new QCheckBox(LightVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(76, 234, 105, 21));
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _106 = new QCheckBox(LightVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(212, 234, 93, 21));
        _106->setLayoutDirection(Qt::RightToLeft);
        _106->setCheckable(true);
        _106->setChecked(false);
        _106->setTristate(false);
        label_4 = new QLabel(LightVerify_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(104, 260, 95, 21));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox = new QGroupBox(LightVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(222, 288, 197, 111));
        _108 = new QCheckBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(true);
        _108->setGeometry(QRect(16, 24, 91, 21));
        _108->setLayoutDirection(Qt::RightToLeft);
        _108->setCheckable(true);
        _108->setChecked(false);
        _108->setTristate(false);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(18, 46, 67, 21));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 76, 65, 21));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _109 = new QDoubleSpinBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(88, 46, 80, 24));
        _109->setDecimals(4);
        _109->setMaximum(10000000.000000000000000);
        _109->setValue(1.000000000000000);
        _110 = new QDoubleSpinBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(88, 76, 80, 24));
        _110->setWrapping(true);
        _110->setDecimals(4);
        _110->setMaximum(1000.000000000000000);
        _110->setValue(1.000000000000000);
        groupBox_2 = new QGroupBox(LightVerify_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(16, 288, 197, 111));
        _111 = new QCheckBox(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(28, 24, 91, 21));
        _111->setLayoutDirection(Qt::RightToLeft);
        _111->setCheckable(true);
        _111->setChecked(false);
        _111->setTristate(false);
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 48, 87, 21));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(6, 78, 91, 21));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _112 = new QDoubleSpinBox(groupBox_2);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(100, 47, 80, 24));
        _112->setDecimals(4);
        _112->setMaximum(180.000000000000000);
        _113 = new QDoubleSpinBox(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(100, 78, 80, 24));
        _113->setWrapping(true);
        _113->setDecimals(4);
        _113->setMaximum(1000.000000000000000);
        _2 = new QPushButton(LightVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(334, 410, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(LightVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(236, 410, 75, 23));
        Defaults = new QGroupBox(LightVerify_Dialog);
        Defaults->setObjectName(QString::fromUtf8("Defaults"));
        Defaults->setGeometry(QRect(16, 8, 400, 87));
        _1002 = new QPushButton(Defaults);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(138, 55, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(Defaults);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(180, 56, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(Defaults);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(368, 28, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(Defaults);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(60, 56, 77, 22));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(Defaults);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(22, 31, 41, 20));
        label_100->setFont(font);
        label_102 = new QLabel(Defaults);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(20, 57, 39, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(Defaults);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(138, 29, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(Defaults);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(60, 30, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(Defaults);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(218, 54, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(Defaults);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(218, 28, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(Defaults);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(296, 54, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(Defaults);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(180, 30, 33, 20));
        label_101->setFont(font);
        _104 = new QDoubleSpinBox(LightVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(208, 206, 80, 24));
        _104->setWrapping(true);
        _104->setDecimals(4);
        _104->setMaximum(1000000.000000000000000);
        _104->setSingleStep(0.100000000000000);
        _104->setValue(0.100000000000000);
        _107 = new QDoubleSpinBox(LightVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(208, 260, 80, 24));
        _107->setDecimals(4);
        _107->setMaximum(1000000.000000000000000);
        _107->setValue(1.000000000000000);
        _2000 = new QGroupBox(LightVerify_Dialog);
        _2000->setObjectName(QString::fromUtf8("_2000"));
        _2000->setEnabled(false);
        _2000->setGeometry(QRect(18, 100, 397, 101));
        label = new QLabel(_2000);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(228, 30, 39, 16));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(_2000);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(232, 52, 35, 16));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_9 = new QLabel(_2000);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 32, 71, 16));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(_2000);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(34, 52, 57, 16));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_11 = new QLabel(_2000);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(200, 70, 67, 16));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        Date = new QLabel(_2000);
        Date->setObjectName(QString::fromUtf8("Date"));
        Date->setGeometry(QRect(272, 30, 85, 16));
        Latitude = new QLabel(_2000);
        Latitude->setObjectName(QString::fromUtf8("Latitude"));
        Latitude->setGeometry(QRect(96, 52, 60, 16));
        Time = new QLabel(_2000);
        Time->setObjectName(QString::fromUtf8("Time"));
        Time->setGeometry(QRect(272, 52, 65, 16));
        Longitude = new QLabel(_2000);
        Longitude->setObjectName(QString::fromUtf8("Longitude"));
        Longitude->setGeometry(QRect(96, 32, 60, 16));
        TimeZone = new QLabel(_2000);
        TimeZone->setObjectName(QString::fromUtf8("TimeZone"));
        TimeZone->setGeometry(QRect(270, 70, 41, 16));
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _112);
        QWidget::setTabOrder(_112, _113);
        QWidget::setTabOrder(_113, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _108);
        QWidget::setTabOrder(_108, _111);

        retranslateUi(LightVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(LightVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LightVerify_Dialog)
    {
        LightVerify_Dialog->setWindowTitle(QCoreApplication::translate("LightVerify_Dialog", "Light options", nullptr));
        label_3->setText(QCoreApplication::translate("LightVerify_Dialog", "Intensity", nullptr));
        _105->setText(QCoreApplication::translate("LightVerify_Dialog", "Distant light", nullptr));
        _106->setText(QCoreApplication::translate("LightVerify_Dialog", "White light", nullptr));
        label_4->setText(QCoreApplication::translate("LightVerify_Dialog", "Display size", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LightVerify_Dialog", "Fade parameters", nullptr));
        _108->setText(QCoreApplication::translate("LightVerify_Dialog", "Enable fade", nullptr));
        label_5->setText(QCoreApplication::translate("LightVerify_Dialog", "Distance", nullptr));
        label_6->setText(QCoreApplication::translate("LightVerify_Dialog", "Exponent", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("LightVerify_Dialog", "Spot parameters", nullptr));
        _111->setText(QCoreApplication::translate("LightVerify_Dialog", "Enable spot", nullptr));
        label_7->setText(QCoreApplication::translate("LightVerify_Dialog", "Cone angle", nullptr));
        label_8->setText(QCoreApplication::translate("LightVerify_Dialog", "Concentration", nullptr));
        _2->setText(QCoreApplication::translate("LightVerify_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("LightVerify_Dialog", "OK", nullptr));
        Defaults->setTitle(QCoreApplication::translate("LightVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("LightVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("LightVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("LightVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("LightVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("LightVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("LightVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("LightVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("LightVerify_Dialog", "Layer", nullptr));
        _2000->setTitle(QCoreApplication::translate("LightVerify_Dialog", "Sun Light Properties", nullptr));
        label->setText(QCoreApplication::translate("LightVerify_Dialog", "Date:", nullptr));
        label_2->setText(QCoreApplication::translate("LightVerify_Dialog", "Time:", nullptr));
        label_9->setText(QCoreApplication::translate("LightVerify_Dialog", "Longitude:", nullptr));
        label_10->setText(QCoreApplication::translate("LightVerify_Dialog", "Latitude:", nullptr));
        label_11->setText(QCoreApplication::translate("LightVerify_Dialog", "TIme Zone:", nullptr));
        Date->setText(QCoreApplication::translate("LightVerify_Dialog", "yyyy/mm/dd", nullptr));
        Latitude->setText(QCoreApplication::translate("LightVerify_Dialog", "degrees", nullptr));
        Time->setText(QCoreApplication::translate("LightVerify_Dialog", "hh:mm:ss", nullptr));
        Longitude->setText(QCoreApplication::translate("LightVerify_Dialog", "degrees", nullptr));
        TimeZone->setText(QCoreApplication::translate("LightVerify_Dialog", "00.00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LightVerify_Dialog: public Ui_LightVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTVERIFY_DIALOG_H
