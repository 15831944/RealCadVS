/********************************************************************************
** Form generated from reading UI file 'RULEDSURFACEVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RULEDSURFACEVERIFY_DIALOG_H
#define UI_RULEDSURFACEVERIFY_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_RuledSurfaceVerify_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_109;
    QPushButton *_1;
    QPushButton *_107;
    QGroupBox *groupBox;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QComboBox *_106;
    RCIntegerSpinBox *_104;
    RCIntegerSpinBox *_105;
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

    void setupUi(QDialog *RuledSurfaceVerify_Dialog)
    {
        if (RuledSurfaceVerify_Dialog->objectName().isEmpty())
            RuledSurfaceVerify_Dialog->setObjectName(QString::fromUtf8("RuledSurfaceVerify_Dialog"));
        RuledSurfaceVerify_Dialog->resize(435, 277);
        _2 = new QPushButton(RuledSurfaceVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(270, 236, 75, 24));
        QFont font;
        font.setPointSize(10);
        _2->setFont(font);
        _2->setAutoDefault(false);
        _109 = new QPushButton(RuledSurfaceVerify_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(true);
        _109->setGeometry(QRect(178, 237, 85, 24));
        _109->setFont(font);
        _109->setAutoDefault(false);
        _1 = new QPushButton(RuledSurfaceVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(90, 237, 75, 24));
        _1->setFont(font);
        _107 = new QPushButton(RuledSurfaceVerify_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(117, 204, 201, 24));
        _107->setFont(font);
        _107->setAutoDefault(false);
        groupBox = new QGroupBox(RuledSurfaceVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 88, 395, 111));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(68, 27, 121, 20));
        label_9->setFont(font);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(68, 52, 121, 20));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(68, 77, 121, 20));
        label_11->setFont(font);
        _106 = new QComboBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(192, 74, 129, 26));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _106->setFont(font1);
        _104 = new RCIntegerSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(192, 24, 75, 24));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Lucida Grande"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        _104->setFont(font2);
        _104->setMinimum(2);
        _104->setMaximum(1000);
        _105 = new RCIntegerSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(192, 50, 75, 24));
        _105->setFont(font1);
        _105->setMinimum(2);
        _105->setMaximum(1000);
        groupBox_3 = new QGroupBox(RuledSurfaceVerify_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 5, 397, 80));
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
        label_100->setGeometry(QRect(16, 27, 41, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(16, 51, 43, 20));
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
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _100);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _107);
        QWidget::setTabOrder(_107, _109);
        QWidget::setTabOrder(_109, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(RuledSurfaceVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(RuledSurfaceVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *RuledSurfaceVerify_Dialog)
    {
        RuledSurfaceVerify_Dialog->setWindowTitle(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Ruled Surface Verify", nullptr));
        _2->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Cancel", nullptr));
        _109->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Triangulate", nullptr));
        _1->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "OK", nullptr));
        _107->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Change surface properties", nullptr));
        groupBox->setTitle(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Options", nullptr));
        label_9->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Horizonal mesh", nullptr));
        label_10->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Vertical mesh", nullptr));
        label_11->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Drawing Mode", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("RuledSurfaceVerify_Dialog", "Layer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RuledSurfaceVerify_Dialog: public Ui_RuledSurfaceVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RULEDSURFACEVERIFY_DIALOG_H
