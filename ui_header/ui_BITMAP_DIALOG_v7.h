/********************************************************************************
** Form generated from reading UI file 'BITMAP_DIALOG_v7.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITMAP_DIALOG_V7_H
#define UI_BITMAP_DIALOG_V7_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Bitmap_Dialog
{
public:
    QGroupBox *groupBox;
    QCheckBox *_104;
    QCheckBox *_105;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_107;
    QComboBox *_108;
    QComboBox *_110;
    QComboBox *_109;
    QLineEdit *_200;
    QLineEdit *_201;
    QLabel *label_200;
    QLabel *label_201;
    QComboBox *_1008;
    QComboBox *_1009;
    QComboBox *_1010;
    QGroupBox *groupBox_4;
    QLabel *label_25;
    QCheckBox *_111;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QDoubleSpinBox *_112;
    QDoubleSpinBox *_114;
    QDoubleSpinBox *_113;
    QDoubleSpinBox *_116;
    QDoubleSpinBox *_117;
    QPushButton *_1005;
    QPushButton *_2;
    QPushButton *_1;
    QGraphicsView *_118;
    QGroupBox *groupBox_2;
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
    QPushButton *_119;
    QPushButton *_120;

    void setupUi(QDialog *Bitmap_Dialog)
    {
        if (Bitmap_Dialog->objectName().isEmpty())
            Bitmap_Dialog->setObjectName(QString::fromUtf8("Bitmap_Dialog"));
        Bitmap_Dialog->resize(516, 725);
        Bitmap_Dialog->setMinimumSize(QSize(516, 725));
        Bitmap_Dialog->setMaximumSize(QSize(516, 752));
        QFont font;
        font.setPointSize(8);
        Bitmap_Dialog->setFont(font);
        groupBox = new QGroupBox(Bitmap_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 86, 483, 265));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        _104 = new QCheckBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(106, 22, 151, 20));
        _104->setFont(font1);
        _104->setLayoutDirection(Qt::RightToLeft);
        _105 = new QCheckBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(106, 42, 151, 20));
        _105->setFont(font1);
        _105->setLayoutDirection(Qt::RightToLeft);
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(106, 116, 123, 20));
        label_22->setFont(font1);
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(106, 146, 121, 20));
        label_23->setFont(font1);
        label_23->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_29 = new QLabel(groupBox);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(106, 174, 133, 20));
        label_29->setFont(font1);
        label_29->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(106, 206, 133, 20));
        label_30->setFont(font1);
        label_30->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_31 = new QLabel(groupBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(106, 236, 135, 20));
        label_31->setFont(font1);
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(240, 115, 77, 24));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Lucida Grande"));
        font2.setPointSize(10);
        _106->setFont(font2);
        _106->setDecimals(2);
        _106->setMinimum(0.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _106->setValue(10.000000000000000);
        _107 = new RCDoubleSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(240, 144, 77, 24));
        _107->setFont(font2);
        _107->setDecimals(2);
        _107->setMinimum(0.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _107->setValue(10.000000000000000);
        _108 = new QComboBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(240, 172, 78, 26));
        _110 = new QComboBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(240, 232, 78, 26));
        _109 = new QComboBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(240, 202, 78, 26));
        _200 = new QLineEdit(groupBox);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setEnabled(false);
        _200->setGeometry(QRect(80, 64, 381, 20));
        _200->setReadOnly(true);
        _201 = new QLineEdit(groupBox);
        _201->setObjectName(QString::fromUtf8("_201"));
        _201->setEnabled(false);
        _201->setGeometry(QRect(80, 90, 381, 20));
        _201->setReadOnly(true);
        label_200 = new QLabel(groupBox);
        label_200->setObjectName(QString::fromUtf8("label_200"));
        label_200->setEnabled(true);
        label_200->setGeometry(QRect(18, 64, 53, 20));
        label_200->setFont(font1);
        label_200->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_201 = new QLabel(groupBox);
        label_201->setObjectName(QString::fromUtf8("label_201"));
        label_201->setEnabled(true);
        label_201->setGeometry(QRect(18, 90, 53, 20));
        label_201->setFont(font1);
        label_201->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1008 = new QComboBox(groupBox);
        _1008->setObjectName(QString::fromUtf8("_1008"));
        _1008->setGeometry(QRect(264, 172, 78, 26));
        _1009 = new QComboBox(groupBox);
        _1009->setObjectName(QString::fromUtf8("_1009"));
        _1009->setGeometry(QRect(264, 202, 78, 26));
        _1010 = new QComboBox(groupBox);
        _1010->setObjectName(QString::fromUtf8("_1010"));
        _1010->setGeometry(QRect(264, 232, 78, 26));
        _1010->raise();
        _1009->raise();
        _1008->raise();
        _104->raise();
        _105->raise();
        label_22->raise();
        label_23->raise();
        label_29->raise();
        label_30->raise();
        label_31->raise();
        _106->raise();
        _107->raise();
        _108->raise();
        _110->raise();
        _109->raise();
        _200->raise();
        _201->raise();
        label_200->raise();
        label_201->raise();
        groupBox_4 = new QGroupBox(Bitmap_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(16, 350, 483, 146));
        groupBox_4->setFont(font1);
        label_25 = new QLabel(groupBox_4);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(14, 48, 45, 20));
        label_25->setFont(font1);
        label_25->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _111 = new QCheckBox(groupBox_4);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(110, 20, 147, 20));
        _111->setFont(font1);
        _111->setLayoutDirection(Qt::RightToLeft);
        label_32 = new QLabel(groupBox_4);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(76, 48, 31, 20));
        label_32->setFont(font1);
        label_32->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_33 = new QLabel(groupBox_4);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(194, 48, 41, 20));
        label_33->setFont(font1);
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_34 = new QLabel(groupBox_4);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(322, 48, 41, 20));
        label_34->setFont(font1);
        label_34->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_35 = new QLabel(groupBox_4);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(122, 86, 115, 20));
        label_35->setFont(font1);
        label_35->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_36 = new QLabel(groupBox_4);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(120, 116, 109, 20));
        label_36->setFont(font1);
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _112 = new QDoubleSpinBox(groupBox_4);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(110, 46, 79, 24));
        _112->setFont(font2);
        _112->setMaximum(1.000000000000000);
        _112->setSingleStep(0.100000000000000);
        _114 = new QDoubleSpinBox(groupBox_4);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(368, 46, 77, 24));
        _114->setFont(font2);
        _114->setMaximum(1.000000000000000);
        _114->setSingleStep(0.100000000000000);
        _113 = new QDoubleSpinBox(groupBox_4);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(240, 46, 77, 24));
        _113->setFont(font2);
        _113->setMaximum(1.000000000000000);
        _113->setSingleStep(0.100000000000000);
        _116 = new QDoubleSpinBox(groupBox_4);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(240, 86, 77, 24));
        _116->setFont(font2);
        _116->setMaximum(1.000000000000000);
        _116->setSingleStep(0.100000000000000);
        _117 = new QDoubleSpinBox(groupBox_4);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(240, 115, 77, 24));
        _117->setFont(font2);
        _117->setMaximum(1.000000000000000);
        _117->setSingleStep(0.010000000000000);
        _1005 = new QPushButton(groupBox_4);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(446, 46, 24, 24));
        _1005->setMouseTracking(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1005->setIcon(icon);
        _1005->setIconSize(QSize(20, 20));
        _1005->setAutoDefault(false);
        _1005->setFlat(true);
        _2 = new QPushButton(Bitmap_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(344, 676, 75, 24));
        _2->setFont(font2);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Bitmap_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(98, 676, 75, 24));
        _1->setFont(font2);
        _1->setAutoDefault(false);
        _118 = new QGraphicsView(Bitmap_Dialog);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(14, 498, 485, 167));
        _118->setMinimumSize(QSize(0, 0));
        _118->setMaximumSize(QSize(485, 173));
        _118->setAcceptDrops(false);
        _118->setFrameShape(QFrame::StyledPanel);
        _118->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setInteractive(false);
        _118->setSceneRect(QRectF(0, 0, 485, 172));
        _118->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        groupBox_2 = new QGroupBox(Bitmap_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(16, 4, 483, 82));
        _1002 = new QPushButton(groupBox_2);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(160, 50, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon1);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_2);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setEnabled(false);
        label_103->setGeometry(QRect(204, 52, 41, 20));
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox_2);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(416, 24, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon2);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_2);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(80, 52, 77, 22));
        _102->setFont(font2);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_2);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setEnabled(false);
        label_100->setGeometry(QRect(38, 27, 39, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox_2);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setEnabled(false);
        label_102->setGeometry(QRect(38, 53, 41, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox_2);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(160, 25, 24, 24));
        _1000->setIcon(icon);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_2);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(80, 26, 77, 22));
        _100->setFont(font2);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(246, 50, 77, 22));
        _103->setFont(font2);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_2);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(246, 24, 167, 22));
        _101->setFont(font2);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_2);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(326, 50, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_2);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setEnabled(false);
        label_101->setGeometry(QRect(206, 26, 37, 20));
        label_101->setFont(font1);
        _119 = new QPushButton(Bitmap_Dialog);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(180, 676, 75, 24));
        _119->setFont(font1);
        _120 = new QPushButton(Bitmap_Dialog);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setEnabled(true);
        _120->setGeometry(QRect(262, 676, 75, 24));
        _120->setFont(font1);

        retranslateUi(Bitmap_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Bitmap_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Bitmap_Dialog)
    {
        Bitmap_Dialog->setWindowTitle(QCoreApplication::translate("Bitmap_Dialog", "Bitmap options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Bitmap_Dialog", "Standard options", nullptr));
        _104->setText(QCoreApplication::translate("Bitmap_Dialog", "Original shape", nullptr));
        _105->setText(QCoreApplication::translate("Bitmap_Dialog", "Link to filename", nullptr));
        label_22->setText(QCoreApplication::translate("Bitmap_Dialog", "Width", nullptr));
        label_23->setText(QCoreApplication::translate("Bitmap_Dialog", "Height", nullptr));
        label_29->setText(QCoreApplication::translate("Bitmap_Dialog", "Horizontal justification", nullptr));
        label_30->setText(QCoreApplication::translate("Bitmap_Dialog", "Vertical justification", nullptr));
        label_31->setText(QCoreApplication::translate("Bitmap_Dialog", "Frame style", nullptr));
        _106->setSpecialValueText(QString());
        label_200->setText(QCoreApplication::translate("Bitmap_Dialog", "Filename", nullptr));
        label_201->setText(QCoreApplication::translate("Bitmap_Dialog", "Origin", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Bitmap_Dialog", "Transparency options", nullptr));
        label_25->setText(QCoreApplication::translate("Bitmap_Dialog", "Colour", nullptr));
        _111->setText(QCoreApplication::translate("Bitmap_Dialog", "Transparent bitmap", nullptr));
        label_32->setText(QCoreApplication::translate("Bitmap_Dialog", "Red", nullptr));
        label_33->setText(QCoreApplication::translate("Bitmap_Dialog", "Green", nullptr));
        label_34->setText(QCoreApplication::translate("Bitmap_Dialog", "Blue", nullptr));
        label_35->setText(QCoreApplication::translate("Bitmap_Dialog", "Transparency range", nullptr));
        label_36->setText(QCoreApplication::translate("Bitmap_Dialog", "Opacity", nullptr));
#if QT_CONFIG(tooltip)
        _1005->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1005->setText(QString());
        _2->setText(QCoreApplication::translate("Bitmap_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Bitmap_Dialog", "OK", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Bitmap_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("Bitmap_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("Bitmap_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("Bitmap_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("Bitmap_Dialog", "Layer", nullptr));
        _119->setText(QCoreApplication::translate("Bitmap_Dialog", "Clone", nullptr));
        _120->setText(QCoreApplication::translate("Bitmap_Dialog", "Clone-All", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Bitmap_Dialog: public Ui_Bitmap_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITMAP_DIALOG_V7_H
