/********************************************************************************
** Form generated from reading UI file 'ELLIPSEVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ELLIPSEVERIFY_DIALOG_H
#define UI_ELLIPSEVERIFY_DIALOG_H

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

class Ui_EllipseVerify_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_46;
    QLineEdit *_104;
    QLineEdit *_105;
    QLabel *label_47;
    QLineEdit *_106;
    QLabel *label_48;
    QLineEdit *_107;
    QLabel *label_49;
    QLabel *label_50;
    QLineEdit *_108;
    QLineEdit *_110;
    QLineEdit *_109;
    QLabel *label_51;
    QLabel *label_52;
    QLabel *label_53;
    QLineEdit *_111;
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

    void setupUi(QDialog *EllipseVerify_Dialog)
    {
        if (EllipseVerify_Dialog->objectName().isEmpty())
            EllipseVerify_Dialog->setObjectName(QString::fromUtf8("EllipseVerify_Dialog"));
        EllipseVerify_Dialog->resize(413, 313);
        EllipseVerify_Dialog->setMinimumSize(QSize(413, 313));
        EllipseVerify_Dialog->setMaximumSize(QSize(413, 313));
        QFont font;
        font.setPointSize(8);
        EllipseVerify_Dialog->setFont(font);
        _2 = new QPushButton(EllipseVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(310, 280, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(EllipseVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(220, 280, 75, 23));
        label_46 = new QLabel(EllipseVerify_Dialog);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(20, 100, 71, 20));
        QFont font1;
        font1.setPointSize(10);
        label_46->setFont(font1);
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QLineEdit(EllipseVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(false);
        _104->setGeometry(QRect(100, 100, 291, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Lucida Grande"));
        font2.setPointSize(10);
        _104->setFont(font2);
        _104->setReadOnly(true);
        _105 = new QLineEdit(EllipseVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(false);
        _105->setGeometry(QRect(100, 130, 291, 20));
        _105->setFont(font2);
        _105->setReadOnly(true);
        label_47 = new QLabel(EllipseVerify_Dialog);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(20, 130, 71, 20));
        label_47->setFont(font1);
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QLineEdit(EllipseVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(false);
        _106->setGeometry(QRect(100, 160, 291, 20));
        _106->setFont(font2);
        _106->setReadOnly(true);
        label_48 = new QLabel(EllipseVerify_Dialog);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(20, 160, 71, 20));
        label_48->setFont(font1);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _107 = new QLineEdit(EllipseVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(false);
        _107->setGeometry(QRect(100, 190, 81, 20));
        _107->setFont(font2);
        _107->setReadOnly(true);
        label_49 = new QLabel(EllipseVerify_Dialog);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setGeometry(QRect(20, 190, 71, 20));
        label_49->setFont(font1);
        label_49->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_50 = new QLabel(EllipseVerify_Dialog);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(230, 190, 71, 20));
        label_50->setFont(font1);
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _108 = new QLineEdit(EllipseVerify_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(false);
        _108->setGeometry(QRect(310, 190, 81, 20));
        _108->setFont(font2);
        _108->setReadOnly(true);
        _110 = new QLineEdit(EllipseVerify_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(false);
        _110->setGeometry(QRect(310, 220, 81, 20));
        _110->setFont(font2);
        _110->setReadOnly(true);
        _109 = new QLineEdit(EllipseVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(false);
        _109->setGeometry(QRect(100, 220, 81, 20));
        _109->setFont(font2);
        _109->setReadOnly(true);
        label_51 = new QLabel(EllipseVerify_Dialog);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(230, 220, 71, 20));
        label_51->setFont(font1);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_52 = new QLabel(EllipseVerify_Dialog);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(20, 220, 71, 20));
        label_52->setFont(font1);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_53 = new QLabel(EllipseVerify_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(20, 250, 71, 20));
        label_53->setFont(font1);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _111 = new QLineEdit(EllipseVerify_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(false);
        _111->setGeometry(QRect(100, 250, 81, 20));
        _111->setFont(font2);
        _111->setReadOnly(true);
        groupBox = new QGroupBox(EllipseVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 6, 381, 87));
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
        _102->setFont(font2);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(10, 31, 39, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(10, 57, 37, 20));
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
        _100->setFont(font2);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(196, 54, 77, 22));
        _103->setFont(font2);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(196, 28, 149, 22));
        _101->setFont(font2);
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

        retranslateUi(EllipseVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(EllipseVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *EllipseVerify_Dialog)
    {
        EllipseVerify_Dialog->setWindowTitle(QCoreApplication::translate("EllipseVerify_Dialog", "Ellipse verify", nullptr));
        _2->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("EllipseVerify_Dialog", "OK", nullptr));
        label_46->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Origin", nullptr));
        label_47->setText(QCoreApplication::translate("EllipseVerify_Dialog", "X axis", nullptr));
        label_48->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Y axis", nullptr));
        label_49->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Major axis", nullptr));
        label_50->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Minor axis", nullptr));
        label_51->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Sweep angle", nullptr));
        label_52->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Start angle", nullptr));
        label_53->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Length", nullptr));
        groupBox->setTitle(QCoreApplication::translate("EllipseVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("EllipseVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("EllipseVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("EllipseVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("EllipseVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("EllipseVerify_Dialog", "Layer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EllipseVerify_Dialog: public Ui_EllipseVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ELLIPSEVERIFY_DIALOG_H
