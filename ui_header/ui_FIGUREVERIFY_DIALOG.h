/********************************************************************************
** Form generated from reading UI file 'FIGUREVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGUREVERIFY_DIALOG_H
#define UI_FIGUREVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_FigureVerify_Dialog
{
public:
    QPushButton *_1;
    QPushButton *_114;
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
    QGroupBox *groupBox;
    QLineEdit *_104;
    QLineEdit *_105;
    QLabel *label_50;
    QLabel *label_51;
    QCheckBox *_119;
    QCheckBox *_117;
    QCheckBox *_116;
    QCheckBox *_115;
    QCheckBox *_113;
    QLabel *label_57;
    QLineEdit *_118;
    QLabel *label_56;
    QLabel *label_54;
    QLabel *label_53;
    QLabel *label_58;
    QLabel *label_55;
    QLabel *label_52;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_107;
    RCDoubleSpinBox *_108;
    RCDoubleSpinBox *_111;
    RCDoubleSpinBox *_110;
    RCDoubleSpinBox *_109;
    QPushButton *_2;

    void setupUi(QDialog *FigureVerify_Dialog)
    {
        if (FigureVerify_Dialog->objectName().isEmpty())
            FigureVerify_Dialog->setObjectName(QString::fromUtf8("FigureVerify_Dialog"));
        FigureVerify_Dialog->resize(437, 395);
        FigureVerify_Dialog->setMinimumSize(QSize(437, 395));
        FigureVerify_Dialog->setMaximumSize(QSize(437, 395));
        QFont font;
        font.setPointSize(10);
        FigureVerify_Dialog->setFont(font);
        _1 = new QPushButton(FigureVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(266, 360, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _1->setFont(font1);
        _114 = new QPushButton(FigureVerify_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(16, 360, 173, 23));
        _114->setFont(font1);
        groupBox_3 = new QGroupBox(FigureVerify_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 10, 397, 80));
        groupBox_3->setFont(font1);
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(134, 49, 24, 24));
        _1002->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(170, 50, 37, 20));
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(360, 24, 24, 24));
        _1001->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(54, 50, 77, 22));
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(18, 27, 39, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(18, 51, 41, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(134, 25, 24, 24));
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
        _100->setGeometry(QRect(54, 26, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(208, 48, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(208, 24, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(true);
        _1003->setGeometry(QRect(288, 48, 24, 24));
        _1003->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(170, 26, 33, 20));
        label_101->setFont(font1);
        groupBox = new QGroupBox(FigureVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 90, 399, 267));
        groupBox->setFont(font1);
        _104 = new QLineEdit(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(92, 26, 291, 20));
        _104->setFont(font1);
        _104->setReadOnly(true);
        _105 = new QLineEdit(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(92, 50, 291, 20));
        _105->setFont(font1);
        _105->setReadOnly(true);
        label_50 = new QLabel(groupBox);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(18, 26, 63, 20));
        label_50->setFont(font1);
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_51 = new QLabel(groupBox);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(16, 50, 69, 20));
        label_51->setFont(font1);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _119 = new QCheckBox(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(110, 124, 159, 21));
        _119->setFont(font1);
        _119->setLayoutDirection(Qt::RightToLeft);
        _119->setCheckable(true);
        _119->setChecked(false);
        _119->setTristate(false);
        _117 = new QCheckBox(groupBox);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setEnabled(true);
        _117->setGeometry(QRect(184, 96, 153, 21));
        _117->setFont(font1);
        _117->setLayoutDirection(Qt::RightToLeft);
        _117->setCheckable(true);
        _117->setChecked(false);
        _117->setTristate(false);
        _116 = new QCheckBox(groupBox);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setEnabled(true);
        _116->setGeometry(QRect(8, 96, 149, 21));
        _116->setFont(font1);
        _116->setLayoutDirection(Qt::RightToLeft);
        _116->setCheckable(true);
        _116->setChecked(false);
        _116->setTristate(false);
        _115 = new QCheckBox(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setEnabled(true);
        _115->setGeometry(QRect(206, 76, 131, 21));
        _115->setFont(font1);
        _115->setLayoutDirection(Qt::RightToLeft);
        _115->setCheckable(true);
        _115->setChecked(false);
        _115->setTristate(false);
        _113 = new QCheckBox(groupBox);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(26, 76, 131, 21));
        _113->setFont(font1);
        _113->setLayoutDirection(Qt::RightToLeft);
        _113->setCheckable(true);
        _113->setChecked(false);
        _113->setTristate(false);
        label_57 = new QLabel(groupBox);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(52, 182, 47, 20));
        label_57->setFont(font1);
        label_57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _118 = new QLineEdit(groupBox);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(62, 152, 321, 20));
        _118->setFont(font1);
        _118->setReadOnly(true);
        label_56 = new QLabel(groupBox);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(200, 184, 60, 20));
        label_56->setFont(font1);
        label_56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_54 = new QLabel(groupBox);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(52, 208, 47, 20));
        label_54->setFont(font1);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_53 = new QLabel(groupBox);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(200, 210, 60, 20));
        label_53->setFont(font1);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_58 = new QLabel(groupBox);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(200, 234, 60, 20));
        label_58->setFont(font1);
        label_58->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_55 = new QLabel(groupBox);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(52, 234, 47, 20));
        label_55->setFont(font1);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_52 = new QLabel(groupBox);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(18, 152, 39, 20));
        label_52->setFont(font1);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(100, 180, 80, 24));
        _106->setFont(font1);
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _107 = new RCDoubleSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(100, 206, 80, 24));
        _107->setFont(font1);
        _107->setMinimum(-100000000.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _108 = new RCDoubleSpinBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(100, 232, 80, 24));
        _108->setFont(font1);
        _108->setMinimum(-100000000.000000000000000);
        _108->setMaximum(100000000.000000000000000);
        _111 = new RCDoubleSpinBox(groupBox);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(262, 232, 80, 24));
        _111->setFont(font1);
        _111->setMinimum(-36000.000000000000000);
        _111->setMaximum(36000.000000000000000);
        _110 = new RCDoubleSpinBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(262, 206, 80, 24));
        _110->setFont(font1);
        _110->setMinimum(-36000.000000000000000);
        _110->setMaximum(36000.000000000000000);
        _109 = new RCDoubleSpinBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(262, 180, 80, 24));
        _109->setFont(font1);
        _109->setMinimum(-36000.000000000000000);
        _109->setMaximum(36000.000000000000000);
        _2 = new QPushButton(FigureVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(350, 360, 75, 23));
        _2->setFont(font1);

        retranslateUi(FigureVerify_Dialog);

        QMetaObject::connectSlotsByName(FigureVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *FigureVerify_Dialog)
    {
        FigureVerify_Dialog->setWindowTitle(QCoreApplication::translate("FigureVerify_Dialog", "Figure verify", nullptr));
        _1->setText(QCoreApplication::translate("FigureVerify_Dialog", "OK", nullptr));
        _114->setText(QCoreApplication::translate("FigureVerify_Dialog", "Modify parametric component", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("FigureVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("FigureVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("FigureVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("FigureVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("FigureVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("FigureVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("FigureVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("FigureVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("FigureVerify_Dialog", "Layer", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FigureVerify_Dialog", "Settings", nullptr));
        label_50->setText(QCoreApplication::translate("FigureVerify_Dialog", "Filename", nullptr));
        label_51->setText(QCoreApplication::translate("FigureVerify_Dialog", "Alt filename", nullptr));
        _119->setText(QCoreApplication::translate("FigureVerify_Dialog", "Orient component to view", nullptr));
        _117->setText(QCoreApplication::translate("FigureVerify_Dialog", "Use original weight", nullptr));
        _116->setText(QCoreApplication::translate("FigureVerify_Dialog", "Use original colour", nullptr));
        _115->setText(QCoreApplication::translate("FigureVerify_Dialog", "Use original style", nullptr));
        _113->setText(QCoreApplication::translate("FigureVerify_Dialog", "Use original layer", nullptr));
        label_57->setText(QCoreApplication::translate("FigureVerify_Dialog", "X scale", nullptr));
        label_56->setText(QCoreApplication::translate("FigureVerify_Dialog", "X rotation", nullptr));
        label_54->setText(QCoreApplication::translate("FigureVerify_Dialog", "Y scale", nullptr));
        label_53->setText(QCoreApplication::translate("FigureVerify_Dialog", "Y rotation", nullptr));
        label_58->setText(QCoreApplication::translate("FigureVerify_Dialog", "Z rotation", nullptr));
        label_55->setText(QCoreApplication::translate("FigureVerify_Dialog", "Z scale", nullptr));
        label_52->setText(QCoreApplication::translate("FigureVerify_Dialog", "Origin", nullptr));
        _2->setText(QCoreApplication::translate("FigureVerify_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FigureVerify_Dialog: public Ui_FigureVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGUREVERIFY_DIALOG_H
