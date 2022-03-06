/********************************************************************************
** Form generated from reading UI file 'TEXTVERIFYCHANGE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTVERIFYCHANGE_DIALOG_H
#define UI_TEXTVERIFYCHANGE_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
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

class Ui_TextVerifyChange_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *_219;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_14;
    QGroupBox *groupBox;
    QCheckBox *_112;
    QCheckBox *_114;
    QCheckBox *_122;
    QCheckBox *_113;
    QCheckBox *_115;
    QCheckBox *_116;
    QCheckBox *_120;
    QCheckBox *_123;
    QPushButton *_1;
    QLineEdit *_119;
    QTextEdit *_104;
    QPushButton *_2;
    RCDoubleSpinBox *_106;
    QComboBox *_109;
    QComboBox *_110;
    RCDoubleSpinBox *_111;
    QSpinBox *_121;
    QFontComboBox *_105;
    RCDoubleSpinBox *_107;
    RCDoubleSpinBox *_108;
    QSpinBox *_100;
    QSpinBox *_102;
    WeightSpinBox *_103;
    QPushButton *_1000;
    QPushButton *_1002;
    QPushButton *_1001;
    QPushButton *_1003;
    LayerSpinBox *_101;
    QPushButton *_1004;

    void setupUi(QDialog *TextVerifyChange_Dialog)
    {
        if (TextVerifyChange_Dialog->objectName().isEmpty())
            TextVerifyChange_Dialog->setObjectName(QString::fromUtf8("TextVerifyChange_Dialog"));
        TextVerifyChange_Dialog->setEnabled(true);
        TextVerifyChange_Dialog->resize(390, 522);
        TextVerifyChange_Dialog->setMinimumSize(QSize(390, 483));
        TextVerifyChange_Dialog->setMaximumSize(QSize(390, 100000));
        label = new QLabel(TextVerifyChange_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(18, 11, 63, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        label->setFont(font);
        label_2 = new QLabel(TextVerifyChange_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(18, 37, 63, 20));
        label_2->setFont(font);
        label_3 = new QLabel(TextVerifyChange_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(150, 36, 39, 20));
        label_3->setFont(font);
        label_4 = new QLabel(TextVerifyChange_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(150, 10, 35, 20));
        label_4->setFont(font);
        _219 = new QLabel(TextVerifyChange_Dialog);
        _219->setObjectName(QString::fromUtf8("_219"));
        _219->setGeometry(QRect(18, 182, 57, 20));
        _219->setFont(font);
        label_6 = new QLabel(TextVerifyChange_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(18, 214, 61, 20));
        label_6->setFont(font);
        label_7 = new QLabel(TextVerifyChange_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(36, 248, 50, 20));
        label_7->setFont(font);
        label_8 = new QLabel(TextVerifyChange_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(36, 275, 50, 20));
        label_8->setFont(font);
        label_9 = new QLabel(TextVerifyChange_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(36, 302, 50, 20));
        label_9->setFont(font);
        label_10 = new QLabel(TextVerifyChange_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(220, 275, 61, 20));
        label_10->setFont(font);
        label_11 = new QLabel(TextVerifyChange_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(220, 302, 71, 20));
        label_11->setFont(font);
        label_12 = new QLabel(TextVerifyChange_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(220, 248, 61, 20));
        label_12->setFont(font);
        label_14 = new QLabel(TextVerifyChange_Dialog);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(124, 354, 71, 20));
        label_14->setFont(font);
        groupBox = new QGroupBox(TextVerifyChange_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 376, 347, 102));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        groupBox->setFont(font1);
        _112 = new QCheckBox(groupBox);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(20, 26, 71, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        _112->setFont(font2);
        _112->setLayoutDirection(Qt::RightToLeft);
        _112->setAutoFillBackground(false);
        _112->setCheckable(true);
        _112->setChecked(false);
        _112->setTristate(false);
        _114 = new QCheckBox(groupBox);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        _114->setGeometry(QRect(20, 49, 71, 20));
        _114->setFont(font2);
        _114->setLayoutDirection(Qt::RightToLeft);
        _114->setAutoFillBackground(false);
        _114->setCheckable(true);
        _114->setChecked(false);
        _114->setTristate(false);
        _122 = new QCheckBox(groupBox);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setEnabled(true);
        _122->setGeometry(QRect(20, 72, 71, 20));
        _122->setFont(font2);
        _122->setLayoutDirection(Qt::RightToLeft);
        _122->setAutoFillBackground(false);
        _122->setCheckable(true);
        _122->setChecked(false);
        _122->setTristate(false);
        _113 = new QCheckBox(groupBox);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(125, 26, 81, 20));
        _113->setFont(font2);
        _113->setLayoutDirection(Qt::RightToLeft);
        _113->setAutoFillBackground(false);
        _113->setCheckable(true);
        _113->setChecked(false);
        _113->setTristate(false);
        _115 = new QCheckBox(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setEnabled(true);
        _115->setGeometry(QRect(135, 49, 71, 20));
        _115->setFont(font2);
        _115->setLayoutDirection(Qt::RightToLeft);
        _115->setAutoFillBackground(false);
        _115->setCheckable(true);
        _115->setChecked(false);
        _115->setTristate(false);
        _116 = new QCheckBox(groupBox);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setEnabled(true);
        _116->setGeometry(QRect(247, 26, 71, 20));
        _116->setFont(font2);
        _116->setLayoutDirection(Qt::RightToLeft);
        _116->setAutoFillBackground(false);
        _116->setCheckable(true);
        _116->setChecked(false);
        _116->setTristate(false);
        _120 = new QCheckBox(groupBox);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setEnabled(true);
        _120->setGeometry(QRect(247, 49, 71, 20));
        _120->setFont(font2);
        _120->setLayoutDirection(Qt::RightToLeft);
        _120->setAutoFillBackground(false);
        _120->setCheckable(true);
        _120->setChecked(false);
        _120->setTristate(false);
        _123 = new QCheckBox(TextVerifyChange_Dialog);
        _123->setObjectName(QString::fromUtf8("_123"));
        _123->setEnabled(true);
        _123->setGeometry(QRect(112, 330, 141, 20));
        _123->setFont(font);
        _123->setLayoutDirection(Qt::RightToLeft);
        _123->setCheckable(true);
        _123->setChecked(false);
        _123->setTristate(false);
        _1 = new QPushButton(TextVerifyChange_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(110, 486, 75, 24));
        _1->setFont(font);
        _119 = new QLineEdit(TextVerifyChange_Dialog);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(76, 182, 289, 20));
        _119->setFont(font);
        _119->setReadOnly(false);
        _104 = new QTextEdit(TextVerifyChange_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(19, 62, 347, 109));
        _104->setLineWrapMode(QTextEdit::NoWrap);
        _104->setReadOnly(false);
        _104->setAcceptRichText(true);
        _2 = new QPushButton(TextVerifyChange_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(196, 486, 75, 23));
        _2->setFont(font);
        _106 = new RCDoubleSpinBox(TextVerifyChange_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(78, 248, 69, 22));
        _106->setFont(font);
        _106->setAccelerated(true);
        _106->setMaximum(100000000.000000000000000);
        _109 = new QComboBox(TextVerifyChange_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(292, 246, 75, 22));
        _109->setFont(font);
        _110 = new QComboBox(TextVerifyChange_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(292, 274, 75, 22));
        _110->setFont(font);
        _111 = new RCDoubleSpinBox(TextVerifyChange_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(294, 302, 71, 22));
        _111->setFont(font);
        _111->setMinimum(-100000000.000000000000000);
        _111->setMaximum(100000000.000000000000000);
        _111->setSingleStep(0.100000000000000);
        _121 = new QSpinBox(TextVerifyChange_Dialog);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(198, 354, 59, 22));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        _121->setFont(font3);
        _121->setWrapping(true);
        _121->setAccelerated(true);
        _105 = new QFontComboBox(TextVerifyChange_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(76, 212, 291, 22));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Lucida Grande"));
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        _105->setFont(font4);
        _105->setEditable(false);
        _105->setCurrentFont(font);
        _107 = new RCDoubleSpinBox(TextVerifyChange_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(78, 274, 69, 22));
        _107->setFont(font);
        _107->setMinimum(-100000000.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _108 = new RCDoubleSpinBox(TextVerifyChange_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(78, 300, 69, 22));
        _108->setFont(font);
        _108->setWrapping(true);
        _108->setAccelerated(true);
        _108->setMinimum(-360.000000000000000);
        _108->setMaximum(360.000000000000000);
        _100 = new QSpinBox(TextVerifyChange_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(60, 10, 59, 22));
        _100->setFont(font);
        _100->setMaximum(255);
        _102 = new QSpinBox(TextVerifyChange_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(60, 36, 59, 22));
        _102->setFont(font);
        _102->setMaximum(255);
        _103 = new WeightSpinBox(TextVerifyChange_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(190, 34, 63, 22));
        _103->setFont(font);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _1000 = new QPushButton(TextVerifyChange_Dialog);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setGeometry(QRect(120, 9, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _1002 = new QPushButton(TextVerifyChange_Dialog);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setGeometry(QRect(120, 35, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon1);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        _1001 = new QPushButton(TextVerifyChange_Dialog);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setGeometry(QRect(338, 8, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon2);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _1003 = new QPushButton(TextVerifyChange_Dialog);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setGeometry(QRect(254, 34, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        _101 = new LayerSpinBox(TextVerifyChange_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(190, 8, 149, 22));
        _101->setFont(font);
        _101->setMaximum(2049);
        _1004 = new QPushButton(TextVerifyChange_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(256, 353, 24, 24));
        _1004->setMouseTracking(false);
        _1004->setIcon(icon);
        _1004->setIconSize(QSize(20, 20));
        _1004->setAutoDefault(false);
        _1004->setFlat(true);
        groupBox->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        _219->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        label_12->raise();
        label_14->raise();
        _123->raise();
        _1->raise();
        _119->raise();
        _104->raise();
        _2->raise();
        _106->raise();
        _109->raise();
        _110->raise();
        _111->raise();
        _121->raise();
        _105->raise();
        _107->raise();
        _108->raise();
        _100->raise();
        _102->raise();
        _103->raise();
        _1000->raise();
        _1002->raise();
        _1001->raise();
        _1003->raise();
        _101->raise();
        _1004->raise();

        retranslateUi(TextVerifyChange_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(TextVerifyChange_Dialog);
    } // setupUi

    void retranslateUi(QDialog *TextVerifyChange_Dialog)
    {
        TextVerifyChange_Dialog->setWindowTitle(QCoreApplication::translate("TextVerifyChange_Dialog", "Text verify", nullptr));
        label->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Colour", nullptr));
        label_2->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Style", nullptr));
        label_3->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Weight", nullptr));
        label_4->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Layer", nullptr));
        _219->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Prompt", nullptr));
        label_6->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Text font", nullptr));
        label_7->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Height", nullptr));
        label_8->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Width", nullptr));
        label_9->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Angle", nullptr));
        label_10->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Vert just", nullptr));
        label_11->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Vert spacing", nullptr));
        label_12->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Horz just", nullptr));
        label_14->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Halo Colour", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TextVerifyChange_Dialog", "Style", nullptr));
        _112->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Bold", nullptr));
        _114->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Italic", nullptr));
        _122->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Halo text", nullptr));
        _113->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Underline", nullptr));
        _115->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Boxed", nullptr));
        _116->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Outline", nullptr));
        _120->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Scaled", nullptr));
        _123->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Text height in points", nullptr));
        _1->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("TextVerifyChange_Dialog", "Cancel", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("TextVerifyChange_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("TextVerifyChange_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("TextVerifyChange_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("TextVerifyChange_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("TextVerifyChange_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TextVerifyChange_Dialog: public Ui_TextVerifyChange_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTVERIFYCHANGE_DIALOG_H
