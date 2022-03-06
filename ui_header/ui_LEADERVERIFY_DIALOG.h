/********************************************************************************
** Form generated from reading UI file 'LEADERVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEADERVERIFY_DIALOG_H
#define UI_LEADERVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_LeaderVerify_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_7;
    QGroupBox *groupBox_3;
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
    QFontComboBox *_1033;
    QSpinBox *_116;
    QLabel *label_8;
    QLabel *label_4;
    QLabel *label_6;
    QComboBox *_104;
    QPushButton *_1004;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_106;
    QLabel *label_3;
    RCDoubleSpinBox *_107;
    QLabel *label_9;
    QLabel *label_5;
    QComboBox *_108;
    QLabel *label_10;
    RCDoubleSpinBox *_109;
    QGroupBox *groupBox_4;
    QCheckBox *_110;
    QCheckBox *_111;
    QCheckBox *_112;
    QCheckBox *_113;
    QCheckBox *_114;
    QCheckBox *_115;
    QCheckBox *_121;
    QGroupBox *groupBox;
    QComboBox *_117;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QCheckBox *_119;
    QLabel *label_14;
    RCDoubleSpinBox *_118;
    RCDoubleSpinBox *_120;
    WeightSpinBox *_122;
    QPushButton *_1005;
    QPushButton *_2000;

    void setupUi(QDialog *LeaderVerify_Dialog)
    {
        if (LeaderVerify_Dialog->objectName().isEmpty())
            LeaderVerify_Dialog->setObjectName(QString::fromUtf8("LeaderVerify_Dialog"));
        LeaderVerify_Dialog->resize(397, 494);
        LeaderVerify_Dialog->setMinimumSize(QSize(397, 494));
        LeaderVerify_Dialog->setMaximumSize(QSize(397, 494));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        LeaderVerify_Dialog->setFont(font);
        _2 = new QPushButton(LeaderVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(308, 462, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _2->setFont(font1);
        _1 = new QPushButton(LeaderVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(226, 462, 75, 23));
        _1->setFont(font1);
        label_7 = new QLabel(LeaderVerify_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(26, 88, 59, 21));
        label_7->setFont(font1);
        groupBox_3 = new QGroupBox(LeaderVerify_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(8, 2, 381, 79));
        groupBox_3->setFont(font1);
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(126, 49, 24, 24));
        _1002->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(152, 50, 43, 20));
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(350, 22, 24, 24));
        _1001->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(56, 50, 70, 22));
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(8, 25, 43, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(8, 51, 45, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(126, 24, 24, 24));
        _1000->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(56, 24, 70, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(202, 48, 70, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(202, 22, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(true);
        _1003->setGeometry(QRect(276, 48, 24, 24));
        _1003->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(152, 24, 41, 20));
        label_101->setFont(font1);
        _1033 = new QFontComboBox(LeaderVerify_Dialog);
        _1033->setObjectName(QString::fromUtf8("_1033"));
        _1033->setGeometry(QRect(90, 84, 283, 28));
        _1033->setFont(font1);
        _1033->setMaxVisibleItems(20);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        _1033->setCurrentFont(font2);
        _116 = new QSpinBox(LeaderVerify_Dialog);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(186, 192, 63, 24));
        _116->setFont(font1);
        label_8 = new QLabel(LeaderVerify_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(26, 170, 46, 21));
        label_8->setFont(font1);
        label_4 = new QLabel(LeaderVerify_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(204, 116, 85, 21));
        label_4->setFont(font1);
        label_6 = new QLabel(LeaderVerify_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(204, 140, 83, 21));
        label_6->setFont(font1);
        _104 = new QComboBox(LeaderVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(292, 114, 81, 22));
        _104->setFont(font1);
        _1004 = new QPushButton(LeaderVerify_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(250, 192, 24, 24));
        _1004->setFont(font1);
        _1004->setMouseTracking(false);
        _1004->setIcon(icon2);
        _1004->setIconSize(QSize(20, 20));
        _1004->setAutoDefault(false);
        _1004->setFlat(true);
        _105 = new RCDoubleSpinBox(LeaderVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(90, 114, 80, 24));
        _105->setFont(font1);
        _105->setMaximum(100000000.000000000000000);
        _106 = new RCDoubleSpinBox(LeaderVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(90, 140, 80, 24));
        _106->setFont(font1);
        _106->setMaximum(100000000.000000000000000);
        _106->setSingleStep(0.100000000000000);
        label_3 = new QLabel(LeaderVerify_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(26, 118, 46, 21));
        label_3->setFont(font1);
        _107 = new RCDoubleSpinBox(LeaderVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(292, 166, 81, 24));
        _107->setFont(font1);
        _107->setMinimum(-10000000.000000000000000);
        _107->setMaximum(10000000.000000000000000);
        _107->setSingleStep(0.100000000000000);
        label_9 = new QLabel(LeaderVerify_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(204, 168, 85, 21));
        label_9->setFont(font1);
        label_5 = new QLabel(LeaderVerify_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(26, 142, 46, 21));
        label_5->setFont(font1);
        _108 = new QComboBox(LeaderVerify_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(292, 138, 81, 22));
        _108->setFont(font1);
        label_10 = new QLabel(LeaderVerify_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(104, 194, 77, 21));
        label_10->setFont(font1);
        _109 = new RCDoubleSpinBox(LeaderVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(90, 166, 80, 24));
        _109->setFont(font1);
        _109->setMinimum(-360.000000000000000);
        _109->setMaximum(360.000000000000000);
        groupBox_4 = new QGroupBox(LeaderVerify_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 214, 377, 84));
        groupBox_4->setFont(font1);
        _110 = new QCheckBox(groupBox_4);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(48, 20, 83, 20));
        _110->setFont(font1);
        _110->setLayoutDirection(Qt::RightToLeft);
        _111 = new QCheckBox(groupBox_4);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(144, 20, 83, 20));
        _111->setFont(font1);
        _111->setLayoutDirection(Qt::RightToLeft);
        _112 = new QCheckBox(groupBox_4);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(242, 20, 69, 20));
        _112->setFont(font1);
        _112->setLayoutDirection(Qt::RightToLeft);
        _113 = new QCheckBox(groupBox_4);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(48, 39, 83, 20));
        _113->setFont(font1);
        _113->setLayoutDirection(Qt::RightToLeft);
        _114 = new QCheckBox(groupBox_4);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(144, 38, 83, 20));
        _114->setFont(font1);
        _114->setLayoutDirection(Qt::RightToLeft);
        _115 = new QCheckBox(groupBox_4);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(242, 38, 69, 20));
        _115->setFont(font1);
        _115->setLayoutDirection(Qt::RightToLeft);
        _121 = new QCheckBox(groupBox_4);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(42, 58, 90, 20));
        _121->setFont(font1);
        _121->setLayoutDirection(Qt::RightToLeft);
        groupBox = new QGroupBox(LeaderVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 302, 375, 153));
        groupBox->setFont(font1);
        _117 = new QComboBox(groupBox);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(158, 20, 119, 22));
        _117->setFont(font1);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(76, 22, 77, 21));
        label_11->setFont(font1);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(76, 46, 81, 21));
        label_12->setFont(font1);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(74, 70, 81, 21));
        label_13->setFont(font1);
        _119 = new QCheckBox(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(66, 96, 153, 21));
        _119->setFont(font1);
        _119->setLayoutDirection(Qt::RightToLeft);
        _119->setCheckable(true);
        _119->setChecked(false);
        _119->setTristate(false);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(74, 122, 81, 21));
        label_14->setFont(font1);
        _118 = new RCDoubleSpinBox(groupBox);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(160, 44, 80, 24));
        _118->setFont(font1);
        _118->setMaximum(100000000.000000000000000);
        _120 = new RCDoubleSpinBox(groupBox);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(160, 70, 80, 24));
        _120->setFont(font1);
        _120->setMaximum(100000000.000000000000000);
        _122 = new WeightSpinBox(groupBox);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setGeometry(QRect(160, 120, 80, 24));
        _122->setFont(font1);
        _122->setMinimum(0.100000000000000);
        _122->setMaximum(3.000000000000000);
        _122->setSingleStep(0.100000000000000);
        _1005 = new QPushButton(groupBox);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(244, 122, 24, 24));
        _1005->setFont(font1);
        _1005->setIcon(icon3);
        _1005->setIconSize(QSize(20, 20));
        _1005->setFlat(true);
        _2000 = new QPushButton(LeaderVerify_Dialog);
        _2000->setObjectName(QString::fromUtf8("_2000"));
        _2000->setGeometry(QRect(8, 462, 81, 28));
        _2000->setAutoDefault(false);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _1033);
        QWidget::setTabOrder(_1033, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _109);
        QWidget::setTabOrder(_109, _104);
        QWidget::setTabOrder(_104, _108);
        QWidget::setTabOrder(_108, _107);
        QWidget::setTabOrder(_107, _116);
        QWidget::setTabOrder(_116, _1004);
        QWidget::setTabOrder(_1004, _110);
        QWidget::setTabOrder(_110, _113);
        QWidget::setTabOrder(_113, _121);
        QWidget::setTabOrder(_121, _111);
        QWidget::setTabOrder(_111, _114);
        QWidget::setTabOrder(_114, _112);
        QWidget::setTabOrder(_112, _115);
        QWidget::setTabOrder(_115, _117);
        QWidget::setTabOrder(_117, _118);
        QWidget::setTabOrder(_118, _120);
        QWidget::setTabOrder(_120, _119);
        QWidget::setTabOrder(_119, _122);
        QWidget::setTabOrder(_122, _1005);
        QWidget::setTabOrder(_1005, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _2000);

        retranslateUi(LeaderVerify_Dialog);

        QMetaObject::connectSlotsByName(LeaderVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LeaderVerify_Dialog)
    {
        LeaderVerify_Dialog->setWindowTitle(QCoreApplication::translate("LeaderVerify_Dialog", "Leader verify", nullptr));
        _2->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("LeaderVerify_Dialog", "OK", nullptr));
        label_7->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Text font", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("LeaderVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Layer", nullptr));
        label_8->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Angle", nullptr));
        label_4->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Horz just", nullptr));
        label_6->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Vert just", nullptr));
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
        label_3->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Height", nullptr));
        label_9->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Vert spacing", nullptr));
        label_5->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Width", nullptr));
        label_10->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Halo colour", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("LeaderVerify_Dialog", "Text Style", nullptr));
        _110->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Bold         ", nullptr));
        _111->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Underline ", nullptr));
        _112->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Outline ", nullptr));
        _113->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Italic         ", nullptr));
        _114->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Boxed      ", nullptr));
        _115->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Halo     ", nullptr));
        _121->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Scale text ", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LeaderVerify_Dialog", "Terminator style", nullptr));
        label_11->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Style", nullptr));
        label_12->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Length", nullptr));
        label_13->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Height", nullptr));
        _119->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Specify terminator weight ", nullptr));
        label_14->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1005->setToolTip(QCoreApplication::translate("LeaderVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1005->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1005->setText(QString());
        _2000->setText(QCoreApplication::translate("LeaderVerify_Dialog", "Change", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LeaderVerify_Dialog: public Ui_LeaderVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEADERVERIFY_DIALOG_H
