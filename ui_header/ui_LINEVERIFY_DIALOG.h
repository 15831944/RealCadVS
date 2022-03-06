/********************************************************************************
** Form generated from reading UI file 'LINEVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEVERIFY_DIALOG_H
#define UI_LINEVERIFY_DIALOG_H

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
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_LineVerify_Dialog
{
public:
    QLineEdit *_106;
    QLabel *label_48;
    QLineEdit *_105;
    QLabel *label_46;
    QLineEdit *_104;
    QLabel *label_47;
    QLabel *label_52;
    QLineEdit *_108;
    QLineEdit *_110;
    QLabel *label_51;
    QPushButton *_1;
    QPushButton *_2;
    QLabel *label_53;
    QLabel *label_54;
    QLineEdit *_109;
    QLineEdit *_111;
    QLabel *label_55;
    RCDoubleSpinBox *_107;
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

    void setupUi(QDialog *LineVerify_Dialog)
    {
        if (LineVerify_Dialog->objectName().isEmpty())
            LineVerify_Dialog->setObjectName(QString::fromUtf8("LineVerify_Dialog"));
        LineVerify_Dialog->resize(462, 323);
        LineVerify_Dialog->setMinimumSize(QSize(462, 323));
        LineVerify_Dialog->setMaximumSize(QSize(462, 323));
        QFont font;
        font.setPointSize(10);
        LineVerify_Dialog->setFont(font);
        _106 = new QLineEdit(LineVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(false);
        _106->setGeometry(QRect(120, 162, 281, 20));
        _106->setReadOnly(true);
        label_48 = new QLabel(LineVerify_Dialog);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(50, 162, 71, 20));
        label_48->setFont(font);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _105 = new QLineEdit(LineVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(false);
        _105->setGeometry(QRect(120, 132, 281, 20));
        _105->setReadOnly(true);
        label_46 = new QLabel(LineVerify_Dialog);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(50, 102, 71, 20));
        label_46->setFont(font);
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QLineEdit(LineVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(false);
        _104->setGeometry(QRect(120, 102, 281, 20));
        _104->setReadOnly(true);
        label_47 = new QLabel(LineVerify_Dialog);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(50, 132, 71, 20));
        label_47->setFont(font);
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_52 = new QLabel(LineVerify_Dialog);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(30, 262, 91, 20));
        label_52->setFont(font);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _108 = new QLineEdit(LineVerify_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(120, 232, 101, 20));
        _108->setReadOnly(true);
        _110 = new QLineEdit(LineVerify_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(120, 262, 101, 20));
        _110->setReadOnly(true);
        label_51 = new QLabel(LineVerify_Dialog);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(30, 232, 91, 20));
        label_51->setFont(font);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1 = new QPushButton(LineVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(276, 292, 75, 23));
        _2 = new QPushButton(LineVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(366, 292, 75, 23));
        _2->setAutoDefault(false);
        label_53 = new QLabel(LineVerify_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(250, 232, 91, 20));
        label_53->setFont(font);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_54 = new QLabel(LineVerify_Dialog);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(250, 262, 91, 20));
        label_54->setFont(font);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _109 = new QLineEdit(LineVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(334, 232, 101, 20));
        _109->setReadOnly(true);
        _111 = new QLineEdit(LineVerify_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(334, 262, 101, 20));
        _111->setReadOnly(true);
        label_55 = new QLabel(LineVerify_Dialog);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(156, 194, 71, 20));
        label_55->setFont(font);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _107 = new RCDoubleSpinBox(LineVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(226, 192, 119, 24));
        _107->setMinimum(-100000000.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        groupBox = new QGroupBox(LineVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(24, 7, 415, 87));
        _1002 = new QPushButton(groupBox);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(138, 55, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(180, 56, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(groupBox);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(368, 28, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(60, 56, 77, 22));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(22, 31, 41, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(20, 57, 39, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(groupBox);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(138, 29, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(60, 30, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(218, 54, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(218, 28, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(296, 54, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(180, 30, 33, 20));
        label_101->setFont(font);

        retranslateUi(LineVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(LineVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LineVerify_Dialog)
    {
        LineVerify_Dialog->setWindowTitle(QCoreApplication::translate("LineVerify_Dialog", "Line verify", nullptr));
        _106->setText(QString());
        label_48->setText(QCoreApplication::translate("LineVerify_Dialog", "Direction", nullptr));
        _105->setText(QString());
        label_46->setText(QCoreApplication::translate("LineVerify_Dialog", "Start", nullptr));
        _104->setText(QString());
        label_47->setText(QCoreApplication::translate("LineVerify_Dialog", "End", nullptr));
        label_52->setText(QCoreApplication::translate("LineVerify_Dialog", "XY angle", nullptr));
        label_51->setText(QCoreApplication::translate("LineVerify_Dialog", "Angle 1", nullptr));
        _1->setText(QCoreApplication::translate("LineVerify_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("LineVerify_Dialog", "Cancel", nullptr));
        label_53->setText(QCoreApplication::translate("LineVerify_Dialog", "Angle 2", nullptr));
        label_54->setText(QCoreApplication::translate("LineVerify_Dialog", "XY angle 2", nullptr));
        label_55->setText(QCoreApplication::translate("LineVerify_Dialog", "Length", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LineVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("LineVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("LineVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("LineVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("LineVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("LineVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("LineVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("LineVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("LineVerify_Dialog", "Layer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LineVerify_Dialog: public Ui_LineVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEVERIFY_DIALOG_H
