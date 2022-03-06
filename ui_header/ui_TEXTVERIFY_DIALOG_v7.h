/********************************************************************************
** Form generated from reading UI file 'TEXTVERIFY_DIALOG_v7.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTVERIFY_DIALOG_V7_H
#define UI_TEXTVERIFY_DIALOG_V7_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_TextVerify_Dialog
{
public:
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
    QTextEdit *_104;
    QLabel *label_119;
    QLineEdit *_119;
    QComboBox *_109;
    RCDoubleSpinBox *_108;
    QLabel *label_10;
    QLabel *label_12;
    RCDoubleSpinBox *_106;
    QLabel *label_11;
    QLabel *label_8;
    QLabel *label_6;
    QLabel *label_9;
    QLabel *label_7;
    QDoubleSpinBox *_111;
    RCDoubleSpinBox *_107;
    QComboBox *_110;
    QFontComboBox *_105;
    QGroupBox *groupBox_2;
    QCheckBox *_112;
    QCheckBox *_114;
    QCheckBox *_122;
    QCheckBox *_113;
    QCheckBox *_115;
    QCheckBox *_116;
    QCheckBox *_120;
    QCheckBox *_123;
    QLabel *label_14;
    QPushButton *_1004;
    QSpinBox *_121;
    QGraphicsView *_124;
    QPushButton *_1;
    QPushButton *_118;
    QPushButton *_2;
    QPushButton *_117;
    QComboBox *_1005;
    QComboBox *_1009;
    QComboBox *_1010;

    void setupUi(QDialog *TextVerify_Dialog)
    {
        if (TextVerify_Dialog->objectName().isEmpty())
            TextVerify_Dialog->setObjectName(QString::fromUtf8("TextVerify_Dialog"));
        TextVerify_Dialog->setEnabled(true);
        TextVerify_Dialog->resize(418, 660);
        TextVerify_Dialog->setMinimumSize(QSize(418, 660));
        TextVerify_Dialog->setMaximumSize(QSize(418, 660));
        groupBox = new QGroupBox(TextVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 11, 381, 87));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        groupBox->setFont(font);
        _1002 = new QPushButton(groupBox);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
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
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
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
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(6, 31, 39, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(6, 57, 39, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
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
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(274, 54, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(158, 30, 33, 20));
        label_101->setFont(font1);
        _104 = new QTextEdit(TextVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(false);
        _104->setGeometry(QRect(16, 104, 383, 83));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        _104->setFont(font2);
        _104->setLineWrapMode(QTextEdit::NoWrap);
        _104->setReadOnly(false);
        _104->setAcceptRichText(true);
        label_119 = new QLabel(TextVerify_Dialog);
        label_119->setObjectName(QString::fromUtf8("label_119"));
        label_119->setEnabled(true);
        label_119->setGeometry(QRect(20, 195, 57, 20));
        label_119->setFont(font2);
        _119 = new QLineEdit(TextVerify_Dialog);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(false);
        _119->setGeometry(QRect(74, 195, 323, 20));
        _119->setFont(font2);
        _119->setReadOnly(false);
        _109 = new QComboBox(TextVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(296, 250, 77, 22));
        _109->setFont(font2);
        _108 = new RCDoubleSpinBox(TextVerify_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(86, 304, 77, 22));
        _108->setFont(font2);
        _108->setWrapping(true);
        _108->setAccelerated(true);
        _108->setMinimum(-360.000000000000000);
        _108->setMaximum(360.000000000000000);
        label_10 = new QLabel(TextVerify_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(224, 279, 61, 20));
        label_10->setFont(font2);
        label_12 = new QLabel(TextVerify_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(224, 252, 61, 20));
        label_12->setFont(font2);
        _106 = new RCDoubleSpinBox(TextVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(86, 252, 77, 22));
        _106->setFont(font2);
        _106->setAccelerated(true);
        _106->setMaximum(100000000.000000000000000);
        label_11 = new QLabel(TextVerify_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(224, 306, 71, 20));
        label_11->setFont(font2);
        label_8 = new QLabel(TextVerify_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(38, 279, 43, 20));
        label_8->setFont(font2);
        label_6 = new QLabel(TextVerify_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(16, 227, 61, 20));
        label_6->setFont(font2);
        label_9 = new QLabel(TextVerify_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(38, 306, 43, 20));
        label_9->setFont(font2);
        label_7 = new QLabel(TextVerify_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(38, 252, 43, 20));
        label_7->setFont(font2);
        _111 = new QDoubleSpinBox(TextVerify_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(298, 306, 77, 22));
        _111->setFont(font2);
        _111->setMinimum(-100000000.000000000000000);
        _111->setMaximum(10000.000000000000000);
        _111->setSingleStep(0.100000000000000);
        _107 = new RCDoubleSpinBox(TextVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(86, 278, 77, 22));
        _107->setFont(font2);
        _107->setWrapping(false);
        _107->setMinimum(0.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _107->setSingleStep(0.100000000000000);
        _110 = new QComboBox(TextVerify_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(296, 278, 77, 22));
        _110->setFont(font2);
        _105 = new QFontComboBox(TextVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(74, 225, 325, 22));
        _105->setFont(font1);
        _105->setEditable(false);
        _105->setMaxVisibleItems(20);
        _105->setCurrentFont(font2);
        groupBox_2 = new QGroupBox(TextVerify_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 354, 381, 93));
        groupBox_2->setFont(font);
        _112 = new QCheckBox(groupBox_2);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(20, 26, 71, 20));
        _112->setFont(font1);
        _112->setLayoutDirection(Qt::RightToLeft);
        _112->setAutoFillBackground(false);
        _112->setCheckable(true);
        _112->setChecked(false);
        _112->setTristate(false);
        _114 = new QCheckBox(groupBox_2);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        _114->setGeometry(QRect(20, 46, 71, 20));
        _114->setFont(font1);
        _114->setLayoutDirection(Qt::RightToLeft);
        _114->setAutoFillBackground(false);
        _114->setCheckable(true);
        _114->setChecked(false);
        _114->setTristate(false);
        _122 = new QCheckBox(groupBox_2);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setEnabled(true);
        _122->setGeometry(QRect(20, 66, 71, 20));
        _122->setFont(font1);
        _122->setLayoutDirection(Qt::RightToLeft);
        _122->setAutoFillBackground(false);
        _122->setCheckable(true);
        _122->setChecked(false);
        _122->setTristate(false);
        _113 = new QCheckBox(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(130, 26, 81, 20));
        _113->setFont(font1);
        _113->setLayoutDirection(Qt::RightToLeft);
        _113->setAutoFillBackground(false);
        _113->setCheckable(true);
        _113->setChecked(false);
        _113->setTristate(false);
        _115 = new QCheckBox(groupBox_2);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setEnabled(true);
        _115->setGeometry(QRect(140, 46, 71, 20));
        _115->setFont(font1);
        _115->setLayoutDirection(Qt::RightToLeft);
        _115->setAutoFillBackground(false);
        _115->setCheckable(true);
        _115->setChecked(false);
        _115->setTristate(false);
        _116 = new QCheckBox(groupBox_2);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setEnabled(true);
        _116->setGeometry(QRect(264, 26, 71, 20));
        _116->setFont(font1);
        _116->setLayoutDirection(Qt::RightToLeft);
        _116->setAutoFillBackground(false);
        _116->setCheckable(true);
        _116->setChecked(false);
        _116->setTristate(false);
        _120 = new QCheckBox(groupBox_2);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setEnabled(true);
        _120->setGeometry(QRect(264, 46, 71, 20));
        _120->setFont(font1);
        _120->setLayoutDirection(Qt::RightToLeft);
        _120->setAutoFillBackground(false);
        _120->setCheckable(true);
        _120->setChecked(false);
        _120->setTristate(false);
        _123 = new QCheckBox(groupBox_2);
        _123->setObjectName(QString::fromUtf8("_123"));
        _123->setEnabled(true);
        _123->setGeometry(QRect(194, 66, 141, 20));
        _123->setFont(font1);
        _123->setLayoutDirection(Qt::RightToLeft);
        _123->setCheckable(true);
        _123->setChecked(false);
        _123->setTristate(false);
        label_14 = new QLabel(TextVerify_Dialog);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(118, 336, 69, 20));
        label_14->setFont(font2);
        _1004 = new QPushButton(TextVerify_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(266, 335, 24, 24));
        _1004->setMouseTracking(false);
        _1004->setIcon(icon2);
        _1004->setIconSize(QSize(20, 20));
        _1004->setAutoDefault(false);
        _1004->setFlat(true);
        _121 = new QSpinBox(TextVerify_Dialog);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(188, 336, 77, 22));
        _121->setFont(font2);
        _121->setWrapping(true);
        _121->setAccelerated(true);
        _121->setMaximum(255);
        _124 = new QGraphicsView(TextVerify_Dialog);
        _124->setObjectName(QString::fromUtf8("_124"));
        _124->setGeometry(QRect(20, 452, 380, 150));
        _124->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _124->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _124->setInteractive(false);
        _124->setSceneRect(QRectF(0, 0, 380, 150));
        _124->setResizeAnchor(QGraphicsView::NoAnchor);
        _1 = new QPushButton(TextVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(50, 610, 75, 24));
        _1->setFont(font2);
        _118 = new QPushButton(TextVerify_Dialog);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setEnabled(true);
        _118->setGeometry(QRect(210, 610, 75, 24));
        _118->setFont(font2);
        _2 = new QPushButton(TextVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(292, 610, 75, 24));
        _2->setFont(font2);
        _117 = new QPushButton(TextVerify_Dialog);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setEnabled(true);
        _117->setGeometry(QRect(130, 610, 75, 24));
        _117->setFont(font2);
        _1005 = new QComboBox(TextVerify_Dialog);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(76, 215, 323, 26));
        _1005->setFont(font2);
        _1009 = new QComboBox(TextVerify_Dialog);
        _1009->setObjectName(QString::fromUtf8("_1009"));
        _1009->setGeometry(QRect(316, 250, 77, 22));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Lucida Grande"));
        font3.setPointSize(10);
        _1009->setFont(font3);
        _1010 = new QComboBox(TextVerify_Dialog);
        _1010->setObjectName(QString::fromUtf8("_1010"));
        _1010->setGeometry(QRect(316, 278, 77, 22));
        _1010->setFont(font3);
        _1010->raise();
        _1009->raise();
        _1005->raise();
        groupBox->raise();
        _104->raise();
        label_119->raise();
        _119->raise();
        _109->raise();
        _108->raise();
        label_10->raise();
        label_12->raise();
        _106->raise();
        label_11->raise();
        label_8->raise();
        label_6->raise();
        label_9->raise();
        label_7->raise();
        _111->raise();
        _107->raise();
        _110->raise();
        _105->raise();
        groupBox_2->raise();
        label_14->raise();
        _1004->raise();
        _121->raise();
        _124->raise();
        _1->raise();
        _118->raise();
        _2->raise();
        _117->raise();
        QWidget::setTabOrder(_104, _119);
        QWidget::setTabOrder(_119, _100);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _1005);
        QWidget::setTabOrder(_1005, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _111);
        QWidget::setTabOrder(_111, _121);
        QWidget::setTabOrder(_121, _1004);
        QWidget::setTabOrder(_1004, _1009);
        QWidget::setTabOrder(_1009, _112);
        QWidget::setTabOrder(_112, _124);
        QWidget::setTabOrder(_124, _114);
        QWidget::setTabOrder(_114, _122);
        QWidget::setTabOrder(_122, _113);
        QWidget::setTabOrder(_113, _115);
        QWidget::setTabOrder(_115, _116);
        QWidget::setTabOrder(_116, _120);
        QWidget::setTabOrder(_120, _123);
        QWidget::setTabOrder(_123, _1);
        QWidget::setTabOrder(_1, _117);
        QWidget::setTabOrder(_117, _118);
        QWidget::setTabOrder(_118, _2);
        QWidget::setTabOrder(_2, _1010);

        retranslateUi(TextVerify_Dialog);

        QMetaObject::connectSlotsByName(TextVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *TextVerify_Dialog)
    {
        TextVerify_Dialog->setWindowTitle(QCoreApplication::translate("TextVerify_Dialog", "Text verify", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TextVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("TextVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("TextVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("TextVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("TextVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("TextVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("TextVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("TextVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("TextVerify_Dialog", "Layer", nullptr));
        label_119->setText(QCoreApplication::translate("TextVerify_Dialog", "Prompt", nullptr));
        _108->setSpecialValueText(QCoreApplication::translate("TextVerify_Dialog", "*Varies*", nullptr));
        label_10->setText(QCoreApplication::translate("TextVerify_Dialog", "Vert just", nullptr));
        label_12->setText(QCoreApplication::translate("TextVerify_Dialog", "Horz just", nullptr));
        _106->setSpecialValueText(QString());
        label_11->setText(QCoreApplication::translate("TextVerify_Dialog", "Vert spacing", nullptr));
        label_8->setText(QCoreApplication::translate("TextVerify_Dialog", "Width", nullptr));
        label_6->setText(QCoreApplication::translate("TextVerify_Dialog", "Text font", nullptr));
        label_9->setText(QCoreApplication::translate("TextVerify_Dialog", "Angle", nullptr));
        label_7->setText(QCoreApplication::translate("TextVerify_Dialog", "Height", nullptr));
        _111->setSpecialValueText(QString());
        _107->setSpecialValueText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("TextVerify_Dialog", "Style", nullptr));
        _112->setText(QCoreApplication::translate("TextVerify_Dialog", "Bold", nullptr));
        _114->setText(QCoreApplication::translate("TextVerify_Dialog", "Italic", nullptr));
        _122->setText(QCoreApplication::translate("TextVerify_Dialog", "Halo text", nullptr));
        _113->setText(QCoreApplication::translate("TextVerify_Dialog", "Underline", nullptr));
        _115->setText(QCoreApplication::translate("TextVerify_Dialog", "Boxed", nullptr));
        _116->setText(QCoreApplication::translate("TextVerify_Dialog", "Outline", nullptr));
        _120->setText(QCoreApplication::translate("TextVerify_Dialog", "Scaled", nullptr));
        _123->setText(QCoreApplication::translate("TextVerify_Dialog", "Text height in points", nullptr));
        label_14->setText(QCoreApplication::translate("TextVerify_Dialog", "Halo Colour", nullptr));
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("TextVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
        _121->setSpecialValueText(QString());
        _1->setText(QCoreApplication::translate("TextVerify_Dialog", "OK", nullptr));
        _118->setText(QCoreApplication::translate("TextVerify_Dialog", "Clone-All", nullptr));
        _2->setText(QCoreApplication::translate("TextVerify_Dialog", "Cancel", nullptr));
        _117->setText(QCoreApplication::translate("TextVerify_Dialog", "Clone", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TextVerify_Dialog: public Ui_TextVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTVERIFY_DIALOG_V7_H
