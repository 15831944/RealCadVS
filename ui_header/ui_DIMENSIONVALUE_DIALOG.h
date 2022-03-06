/********************************************************************************
** Form generated from reading UI file 'DIMENSIONVALUE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONVALUE_DIALOG_H
#define UI_DIMENSIONVALUE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_DimensionValue_Dialog
{
public:
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *_104;
    QComboBox *_105;
    QGroupBox *groupBox;
    QCheckBox *_107;
    QCheckBox *_109;
    QCheckBox *_108;
    QCheckBox *_110;
    QGroupBox *groupBox_2;
    QCheckBox *_111;
    QComboBox *_113;
    QLabel *label_11;
    QComboBox *_1013;
    QPushButton *_2;
    QPushButton *_1;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_106;
    QGraphicsView *_112;
    QFontComboBox *_100;
    QComboBox *_1000;
    QComboBox *_1004;
    QComboBox *_1005;

    void setupUi(QDialog *DimensionValue_Dialog)
    {
        if (DimensionValue_Dialog->objectName().isEmpty())
            DimensionValue_Dialog->setObjectName(QString::fromUtf8("DimensionValue_Dialog"));
        DimensionValue_Dialog->resize(335, 369);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        DimensionValue_Dialog->setFont(font);
        label_4 = new QLabel(DimensionValue_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(16, 10, 65, 20));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5 = new QLabel(DimensionValue_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(16, 36, 61, 21));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(DimensionValue_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(16, 61, 71, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(DimensionValue_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(16, 86, 59, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(DimensionValue_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(162, 88, 85, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9 = new QLabel(DimensionValue_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(162, 36, 85, 21));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_10 = new QLabel(DimensionValue_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(162, 62, 85, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QComboBox(DimensionValue_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(246, 34, 79, 22));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        _104->setFont(font1);
        _105 = new QComboBox(DimensionValue_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(246, 60, 79, 22));
        _105->setFont(font1);
        groupBox = new QGroupBox(DimensionValue_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(12, 114, 153, 71));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setBold(true);
        font2.setWeight(75);
        groupBox->setFont(font2);
        _107 = new QCheckBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(4, 22, 49, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        _107->setFont(font3);
        _107->setLayoutDirection(Qt::RightToLeft);
        _109 = new QCheckBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(4, 44, 49, 21));
        _109->setFont(font3);
        _109->setLayoutDirection(Qt::RightToLeft);
        _108 = new QCheckBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(56, 20, 87, 21));
        _108->setFont(font3);
        _108->setLayoutDirection(Qt::RightToLeft);
        _110 = new QCheckBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(58, 44, 85, 21));
        _110->setFont(font3);
        _110->setLayoutDirection(Qt::RightToLeft);
        groupBox_2 = new QGroupBox(DimensionValue_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(170, 114, 151, 71));
        groupBox_2->setFont(font2);
        _111 = new QCheckBox(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(-2, 20, 65, 21));
        _111->setFont(font3);
        _111->setLayoutDirection(Qt::RightToLeft);
        _113 = new QComboBox(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(72, 42, 75, 22));
        QFont font4;
        font4.setBold(false);
        font4.setWeight(50);
        _113->setFont(font4);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(6, 42, 71, 21));
        label_11->setFont(font3);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1013 = new QComboBox(groupBox_2);
        _1013->setObjectName(QString::fromUtf8("_1013"));
        _1013->setGeometry(QRect(76, 35, 75, 22));
        _1013->setFont(font4);
        _1013->raise();
        _111->raise();
        _113->raise();
        label_11->raise();
        _2 = new QPushButton(DimensionValue_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(236, 336, 75, 23));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(DimensionValue_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(146, 336, 75, 23));
        _1->setFont(font);
        _101 = new RCDoubleSpinBox(DimensionValue_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(76, 34, 79, 24));
        _101->setFont(font1);
        _101->setMaximum(100000000.000000000000000);
        _102 = new RCDoubleSpinBox(DimensionValue_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(76, 60, 79, 24));
        _102->setFont(font1);
        _102->setMinimum(0.000000000000000);
        _102->setMaximum(100000000.000000000000000);
        _102->setSingleStep(0.100000000000000);
        _103 = new RCDoubleSpinBox(DimensionValue_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(76, 86, 79, 24));
        _103->setFont(font1);
        _103->setMinimum(-360.000000000000000);
        _103->setMaximum(360.000000000000000);
        _106 = new RCDoubleSpinBox(DimensionValue_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(246, 86, 79, 24));
        _106->setFont(font1);
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _106->setSingleStep(0.100000000000000);
        _112 = new QGraphicsView(DimensionValue_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(14, 194, 307, 142));
        _112->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _112->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _112->setSceneRect(QRectF(0, 0, 307, 142));
        _112->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _112->setTransformationAnchor(QGraphicsView::NoAnchor);
        _112->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _100 = new QFontComboBox(DimensionValue_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(76, 6, 249, 25));
        _100->setEditable(false);
        _100->setMaxVisibleItems(10);
        _1000 = new QComboBox(DimensionValue_Dialog);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setGeometry(QRect(92, 10, 239, 24));
        _1004 = new QComboBox(DimensionValue_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(252, 39, 79, 22));
        _1004->setFont(font1);
        _1005 = new QComboBox(DimensionValue_Dialog);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(252, 64, 79, 22));
        _1005->setFont(font1);
        _1004->raise();
        _1005->raise();
        _1000->raise();
        label_4->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        _104->raise();
        _105->raise();
        groupBox->raise();
        groupBox_2->raise();
        _2->raise();
        _1->raise();
        _101->raise();
        _102->raise();
        _103->raise();
        _106->raise();
        _112->raise();
        _100->raise();

        retranslateUi(DimensionValue_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionValue_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionValue_Dialog)
    {
        DimensionValue_Dialog->setWindowTitle(QCoreApplication::translate("DimensionValue_Dialog", "Dimension value text options", nullptr));
        label_4->setText(QCoreApplication::translate("DimensionValue_Dialog", "Text font", nullptr));
        label_5->setText(QCoreApplication::translate("DimensionValue_Dialog", "Height", nullptr));
        label_6->setText(QCoreApplication::translate("DimensionValue_Dialog", "Width", nullptr));
        label_7->setText(QCoreApplication::translate("DimensionValue_Dialog", "Angle", nullptr));
        label_8->setText(QCoreApplication::translate("DimensionValue_Dialog", "Vert Spacing", nullptr));
        label_9->setText(QCoreApplication::translate("DimensionValue_Dialog", "Horiz Just", nullptr));
        label_10->setText(QCoreApplication::translate("DimensionValue_Dialog", "Vert Just", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DimensionValue_Dialog", "Style", nullptr));
        _107->setText(QCoreApplication::translate("DimensionValue_Dialog", "Bold", nullptr));
        _109->setText(QCoreApplication::translate("DimensionValue_Dialog", "Italic", nullptr));
        _108->setText(QCoreApplication::translate("DimensionValue_Dialog", "Underline", nullptr));
        _110->setText(QCoreApplication::translate("DimensionValue_Dialog", "Boxed", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("DimensionValue_Dialog", "Value", nullptr));
        _111->setText(QCoreApplication::translate("DimensionValue_Dialog", "Display", nullptr));
        label_11->setText(QCoreApplication::translate("DimensionValue_Dialog", "Placement", nullptr));
        _2->setText(QCoreApplication::translate("DimensionValue_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DimensionValue_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DimensionValue_Dialog: public Ui_DimensionValue_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONVALUE_DIALOG_H
