/********************************************************************************
** Form generated from reading UI file 'SURFACEOFREV_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SURFACEOFREV_DIALOG_H
#define UI_SURFACEOFREV_DIALOG_H

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
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_SurfaceOfRev_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_113;
    QPushButton *_1;
    QPushButton *_112;
    QPushButton *_110;
    QGroupBox *groupBox;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QComboBox *_108;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_105;
    RCIntegerSpinBox *_106;
    RCIntegerSpinBox *_107;
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
    QPushButton *_111;
    QGraphicsView *_109;

    void setupUi(QDialog *SurfaceOfRev_Dialog)
    {
        if (SurfaceOfRev_Dialog->objectName().isEmpty())
            SurfaceOfRev_Dialog->setObjectName(QString::fromUtf8("SurfaceOfRev_Dialog"));
        SurfaceOfRev_Dialog->resize(419, 477);
        SurfaceOfRev_Dialog->setMinimumSize(QSize(419, 477));
        SurfaceOfRev_Dialog->setMaximumSize(QSize(419, 477));
        QFont font;
        font.setPointSize(10);
        SurfaceOfRev_Dialog->setFont(font);
        _2 = new QPushButton(SurfaceOfRev_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(298, 442, 75, 24));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _113 = new QPushButton(SurfaceOfRev_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(216, 442, 75, 24));
        _113->setFont(font);
        _113->setAutoDefault(false);
        _1 = new QPushButton(SurfaceOfRev_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(56, 442, 75, 24));
        _1->setFont(font);
        _112 = new QPushButton(SurfaceOfRev_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(136, 442, 75, 24));
        _112->setFont(font);
        _112->setAutoDefault(false);
        _110 = new QPushButton(SurfaceOfRev_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(true);
        _110->setGeometry(QRect(56, 412, 201, 24));
        _110->setFont(font);
        _110->setAutoDefault(false);
        groupBox = new QGroupBox(SurfaceOfRev_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(12, 93, 395, 158));
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
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _108->setFont(font1);
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(192, 24, 75, 24));
        _104->setFont(font1);
        _104->setDecimals(2);
        _104->setMinimum(-100000.000000000000000);
        _104->setMaximum(100000.000000000000000);
        _104->setValue(360.000000000000000);
        _105 = new RCDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(192, 50, 75, 24));
        _105->setFont(font1);
        _105->setDecimals(2);
        _105->setMinimum(-1000000000.000000000000000);
        _105->setMaximum(100000000.000000000000000);
        _106 = new RCIntegerSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(192, 76, 75, 24));
        _106->setFont(font1);
        _106->setMinimum(2);
        _106->setMaximum(1000);
        _107 = new RCIntegerSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(192, 102, 75, 24));
        _107->setFont(font1);
        _107->setMinimum(2);
        _107->setMaximum(1000);
        groupBox_3 = new QGroupBox(SurfaceOfRev_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(12, 10, 397, 80));
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
        label_103->setGeometry(QRect(166, 50, 41, 20));
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
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(14, 27, 43, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(14, 51, 45, 20));
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
        label_101->setGeometry(QRect(166, 26, 37, 20));
        label_101->setFont(font);
        _111 = new QPushButton(SurfaceOfRev_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(288, 412, 85, 24));
        _111->setFont(font);
        _111->setAutoDefault(false);
        _109 = new QGraphicsView(SurfaceOfRev_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(12, 260, 397, 147));
        _109->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _109->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(SurfaceOfRev_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SurfaceOfRev_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SurfaceOfRev_Dialog)
    {
        SurfaceOfRev_Dialog->setWindowTitle(QCoreApplication::translate("SurfaceOfRev_Dialog", "Surface of revolution options", nullptr));
        _2->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Cancel", nullptr));
        _113->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Clone-All", nullptr));
        _1->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "OK", nullptr));
        _112->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Clone", nullptr));
        _110->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Change surface properties", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SurfaceOfRev_Dialog", "Options", nullptr));
        label_8->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Shear along axis", nullptr));
        label_7->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Rotation Angle", nullptr));
        label_9->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Horizonal mesh", nullptr));
        label_10->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Vertical mesh", nullptr));
        label_11->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Drawing Mode", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("SurfaceOfRev_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("SurfaceOfRev_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("SurfaceOfRev_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("SurfaceOfRev_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("SurfaceOfRev_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Layer", nullptr));
        _111->setText(QCoreApplication::translate("SurfaceOfRev_Dialog", "Triangulate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SurfaceOfRev_Dialog: public Ui_SurfaceOfRev_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURFACEOFREV_DIALOG_H
