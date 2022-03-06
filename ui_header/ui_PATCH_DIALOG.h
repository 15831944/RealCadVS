/********************************************************************************
** Form generated from reading UI file 'PATCH_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PATCH_DIALOG_H
#define UI_PATCH_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Patch_Dialog
{
public:
    QPushButton *_1;
    QPushButton *_113;
    QPushButton *_114;
    QPushButton *_2;
    QGroupBox *groupBox;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QComboBox *_108;
    QSpinBox *_104;
    QSpinBox *_105;
    QSpinBox *_106;
    QSpinBox *_107;
    QPushButton *_112;
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
    QPushButton *_110;
    QPushButton *_111;
    QGraphicsView *_109;

    void setupUi(QDialog *Patch_Dialog)
    {
        if (Patch_Dialog->objectName().isEmpty())
            Patch_Dialog->setObjectName(QString::fromUtf8("Patch_Dialog"));
        Patch_Dialog->resize(426, 476);
        QFont font;
        font.setPointSize(10);
        Patch_Dialog->setFont(font);
        _1 = new QPushButton(Patch_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(46, 442, 75, 24));
        _1->setFont(font);
        _113 = new QPushButton(Patch_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(126, 442, 75, 24));
        _113->setFont(font);
        _114 = new QPushButton(Patch_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        _114->setGeometry(QRect(208, 442, 75, 24));
        _114->setFont(font);
        _2 = new QPushButton(Patch_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(290, 442, 75, 24));
        _2->setFont(font);
        groupBox = new QGroupBox(Patch_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 95, 397, 158));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(68, 52, 121, 20));
        label_8->setFont(font);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(67, 25, 121, 20));
        label_7->setFont(font);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(68, 79, 121, 20));
        label_9->setFont(font);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(68, 104, 121, 20));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(68, 129, 121, 20));
        label_11->setFont(font);
        _108 = new QComboBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(192, 126, 129, 26));
        _104 = new QSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(192, 24, 75, 24));
        _104->setMinimum(2);
        _104->setMaximum(10);
        _105 = new QSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(192, 50, 75, 24));
        _105->setMinimum(2);
        _105->setMaximum(10);
        _106 = new QSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(192, 76, 75, 24));
        _106->setMinimum(2);
        _106->setMaximum(1000);
        _107 = new QSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(192, 102, 75, 24));
        _107->setMinimum(2);
        _107->setMaximum(1000);
        _112 = new QPushButton(Patch_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(330, 414, 83, 24));
        _112->setFont(font);
        _112->setAutoDefault(false);
        groupBox_3 = new QGroupBox(Patch_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(16, 12, 397, 80));
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(134, 49, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(170, 50, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(360, 24, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(54, 50, 77, 22));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(18, 27, 39, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(18, 51, 41, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(134, 25, 24, 24));
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
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(170, 26, 33, 20));
        label_101->setFont(font);
        _110 = new QPushButton(Patch_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(true);
        _110->setGeometry(QRect(16, 414, 201, 24));
        _110->setFont(font);
        _110->setAutoDefault(false);
        _111 = new QPushButton(Patch_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(236, 414, 81, 24));
        _111->setFont(font);
        _111->setAutoDefault(false);
        _109 = new QGraphicsView(Patch_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(16, 260, 397, 147));
        _109->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _109->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(Patch_Dialog);

        QMetaObject::connectSlotsByName(Patch_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Patch_Dialog)
    {
        Patch_Dialog->setWindowTitle(QCoreApplication::translate("Patch_Dialog", "Patch Options", nullptr));
        _1->setText(QCoreApplication::translate("Patch_Dialog", "OK", nullptr));
        _113->setText(QCoreApplication::translate("Patch_Dialog", "Clone", nullptr));
        _114->setText(QCoreApplication::translate("Patch_Dialog", "Clone-All", nullptr));
        _2->setText(QCoreApplication::translate("Patch_Dialog", "Cancel", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Patch_Dialog", "Options", nullptr));
        label_8->setText(QCoreApplication::translate("Patch_Dialog", "Vertical degree", nullptr));
        label_7->setText(QCoreApplication::translate("Patch_Dialog", "Horizontal degree", nullptr));
        label_9->setText(QCoreApplication::translate("Patch_Dialog", "Horizonal mesh", nullptr));
        label_10->setText(QCoreApplication::translate("Patch_Dialog", "Vertical mesh", nullptr));
        label_11->setText(QCoreApplication::translate("Patch_Dialog", "Drawing Mode", nullptr));
        _112->setText(QCoreApplication::translate("Patch_Dialog", "Triangulate", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Patch_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Patch_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("Patch_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Patch_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("Patch_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("Patch_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("Patch_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Patch_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("Patch_Dialog", "Layer", nullptr));
        _110->setText(QCoreApplication::translate("Patch_Dialog", "Change surface properties", nullptr));
        _111->setText(QCoreApplication::translate("Patch_Dialog", "Compare", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Patch_Dialog: public Ui_Patch_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PATCH_DIALOG_H
