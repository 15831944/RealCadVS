/********************************************************************************
** Form generated from reading UI file 'CURVEVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CURVEVERIFY_DIALOG_H
#define UI_CURVEVERIFY_DIALOG_H

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

class Ui_CurveVerify_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLineEdit *_104;
    QLabel *label_46;
    QLabel *label_47;
    QLineEdit *_105;
    QLabel *label_48;
    QLineEdit *_106;
    QLabel *label_49;
    QLineEdit *_107;
    QLineEdit *_109;
    QLabel *label_50;
    QLineEdit *_108;
    QLabel *label_51;
    QLabel *label_52;
    QLineEdit *_110;
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

    void setupUi(QDialog *CurveVerify_Dialog)
    {
        if (CurveVerify_Dialog->objectName().isEmpty())
            CurveVerify_Dialog->setObjectName(QString::fromUtf8("CurveVerify_Dialog"));
        CurveVerify_Dialog->resize(415, 358);
        CurveVerify_Dialog->setMinimumSize(QSize(415, 358));
        CurveVerify_Dialog->setMaximumSize(QSize(415, 358));
        QFont font;
        font.setPointSize(8);
        CurveVerify_Dialog->setFont(font);
        _2 = new QPushButton(CurveVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(284, 322, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(CurveVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(194, 322, 75, 23));
        _104 = new QLineEdit(CurveVerify_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(false);
        _104->setGeometry(QRect(282, 112, 81, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _104->setFont(font1);
        _104->setReadOnly(true);
        label_46 = new QLabel(CurveVerify_Dialog);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(44, 112, 71, 20));
        QFont font2;
        font2.setPointSize(10);
        label_46->setFont(font2);
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_47 = new QLabel(CurveVerify_Dialog);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(44, 142, 151, 20));
        label_47->setFont(font2);
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _105 = new QLineEdit(CurveVerify_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(false);
        _105->setGeometry(QRect(282, 142, 81, 20));
        _105->setFont(font1);
        _105->setReadOnly(true);
        label_48 = new QLabel(CurveVerify_Dialog);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(44, 172, 57, 20));
        label_48->setFont(font2);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QLineEdit(CurveVerify_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(false);
        _106->setGeometry(QRect(108, 172, 255, 20));
        _106->setFont(font1);
        _106->setReadOnly(true);
        label_49 = new QLabel(CurveVerify_Dialog);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setGeometry(QRect(44, 202, 63, 20));
        label_49->setFont(font2);
        label_49->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _107 = new QLineEdit(CurveVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(false);
        _107->setGeometry(QRect(108, 202, 255, 20));
        _107->setFont(font1);
        _107->setReadOnly(true);
        _109 = new QLineEdit(CurveVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(false);
        _109->setGeometry(QRect(108, 262, 255, 20));
        _109->setFont(font1);
        _109->setReadOnly(true);
        label_50 = new QLabel(CurveVerify_Dialog);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(44, 232, 59, 20));
        label_50->setFont(font2);
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _108 = new QLineEdit(CurveVerify_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(false);
        _108->setGeometry(QRect(108, 232, 255, 20));
        _108->setFont(font1);
        _108->setReadOnly(true);
        label_51 = new QLabel(CurveVerify_Dialog);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(44, 262, 59, 20));
        label_51->setFont(font2);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_52 = new QLabel(CurveVerify_Dialog);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(44, 292, 61, 20));
        label_52->setFont(font2);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _110 = new QLineEdit(CurveVerify_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(false);
        _110->setGeometry(QRect(108, 292, 255, 20));
        _110->setFont(font1);
        _110->setReadOnly(true);
        groupBox = new QGroupBox(CurveVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(18, 12, 381, 87));
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
        label_103->setFont(font2);
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
        label_100->setGeometry(QRect(10, 31, 37, 20));
        label_100->setFont(font2);
        label_102 = new QLabel(groupBox);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(10, 57, 35, 20));
        label_102->setFont(font2);
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
        label_101->setFont(font2);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _110);

        retranslateUi(CurveVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(CurveVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *CurveVerify_Dialog)
    {
        CurveVerify_Dialog->setWindowTitle(QCoreApplication::translate("CurveVerify_Dialog", "Curve verify", nullptr));
        _2->setText(QCoreApplication::translate("CurveVerify_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("CurveVerify_Dialog", "OK", nullptr));
        label_46->setText(QCoreApplication::translate("CurveVerify_Dialog", "Degree", nullptr));
        label_47->setText(QCoreApplication::translate("CurveVerify_Dialog", "Number of control points", nullptr));
        label_48->setText(QCoreApplication::translate("CurveVerify_Dialog", "Start", nullptr));
        label_49->setText(QCoreApplication::translate("CurveVerify_Dialog", "End", nullptr));
        label_50->setText(QCoreApplication::translate("CurveVerify_Dialog", "Centre", nullptr));
        label_51->setText(QCoreApplication::translate("CurveVerify_Dialog", "Minimum", nullptr));
        label_52->setText(QCoreApplication::translate("CurveVerify_Dialog", "Maximum", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CurveVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("CurveVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("CurveVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("CurveVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("CurveVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("CurveVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("CurveVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("CurveVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("CurveVerify_Dialog", "Layer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CurveVerify_Dialog: public Ui_CurveVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CURVEVERIFY_DIALOG_H
