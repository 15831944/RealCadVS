/********************************************************************************
** Form generated from reading UI file 'DIMENSIONPREFIX_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONPREFIX_DIALOG_H
#define UI_DIMENSIONPREFIX_DIALOG_H

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

class Ui_DimensionPrefix_Dialog
{
public:
    QLabel *label_45;
    QLabel *label_53;
    QLabel *label_54;
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_56;
    QLabel *label_57;
    QLabel *label_58;
    QLabel *label_59;
    QComboBox *_104;
    QComboBox *_105;
    QGroupBox *groupBox;
    QCheckBox *_107;
    QCheckBox *_109;
    QCheckBox *_110;
    QCheckBox *_108;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_106;
    QGraphicsView *_111;
    QFontComboBox *_100;
    QComboBox *_1000;
    QComboBox *_1004;
    QComboBox *_1005;

    void setupUi(QDialog *DimensionPrefix_Dialog)
    {
        if (DimensionPrefix_Dialog->objectName().isEmpty())
            DimensionPrefix_Dialog->setObjectName(QString::fromUtf8("DimensionPrefix_Dialog"));
        DimensionPrefix_Dialog->resize(341, 369);
        QFont font;
        font.setPointSize(10);
        DimensionPrefix_Dialog->setFont(font);
        label_45 = new QLabel(DimensionPrefix_Dialog);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(18, 10, 69, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        label_45->setFont(font1);
        label_45->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_53 = new QLabel(DimensionPrefix_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(18, 36, 69, 20));
        label_53->setFont(font1);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_54 = new QLabel(DimensionPrefix_Dialog);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(18, 62, 67, 20));
        label_54->setFont(font1);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DimensionPrefix_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(246, 334, 75, 27));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(DimensionPrefix_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(156, 334, 75, 27));
        _1->setFont(font1);
        _1->setAutoDefault(true);
        label_56 = new QLabel(DimensionPrefix_Dialog);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(18, 88, 67, 20));
        label_56->setFont(font1);
        label_56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_57 = new QLabel(DimensionPrefix_Dialog);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(160, 38, 83, 20));
        label_57->setFont(font1);
        label_57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_58 = new QLabel(DimensionPrefix_Dialog);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(160, 88, 85, 20));
        label_58->setFont(font1);
        label_58->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_59 = new QLabel(DimensionPrefix_Dialog);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setGeometry(QRect(160, 62, 83, 20));
        label_59->setFont(font1);
        label_59->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QComboBox(DimensionPrefix_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(242, 36, 81, 22));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        _104->setFont(font2);
        _105 = new QComboBox(DimensionPrefix_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(242, 60, 81, 22));
        _105->setFont(font2);
        groupBox = new QGroupBox(DimensionPrefix_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(14, 112, 317, 72));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        groupBox->setFont(font3);
        _107 = new QCheckBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(68, 21, 59, 21));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        _107->setFont(font4);
        _107->setLayoutDirection(Qt::RightToLeft);
        _109 = new QCheckBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(68, 43, 59, 21));
        _109->setFont(font4);
        _109->setLayoutDirection(Qt::RightToLeft);
        _110 = new QCheckBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(154, 42, 81, 21));
        _110->setFont(font4);
        _110->setLayoutDirection(Qt::RightToLeft);
        _108 = new QCheckBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(154, 22, 81, 21));
        _108->setFont(font4);
        _108->setLayoutDirection(Qt::RightToLeft);
        _101 = new RCDoubleSpinBox(DimensionPrefix_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(84, 34, 70, 24));
        _101->setFont(font2);
        _101->setMaximum(100000.000000000000000);
        _102 = new RCDoubleSpinBox(DimensionPrefix_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(84, 60, 70, 24));
        _102->setFont(font2);
        _102->setMaximum(100000.000000000000000);
        _102->setSingleStep(0.100000000000000);
        _103 = new RCDoubleSpinBox(DimensionPrefix_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(84, 86, 70, 24));
        _103->setFont(font2);
        _103->setMinimum(-360.000000000000000);
        _103->setMaximum(360.000000000000000);
        _106 = new RCDoubleSpinBox(DimensionPrefix_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(242, 86, 81, 24));
        _106->setFont(font2);
        _106->setMinimum(-10000.000000000000000);
        _106->setMaximum(10000.000000000000000);
        _106->setSingleStep(0.100000000000000);
        _111 = new QGraphicsView(DimensionPrefix_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(10, 192, 319, 139));
        _111->setFont(font2);
        _111->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _111->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _111->setSceneRect(QRectF(0, 0, 319, 139));
        _111->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _111->setTransformationAnchor(QGraphicsView::NoAnchor);
        _111->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _100 = new QFontComboBox(DimensionPrefix_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(82, 6, 241, 25));
        _100->setFont(font2);
        _100->setEditable(false);
        _100->setCurrentFont(font2);
        _1000 = new QComboBox(DimensionPrefix_Dialog);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setGeometry(QRect(90, 1, 241, 25));
        _1004 = new QComboBox(DimensionPrefix_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(250, 31, 81, 22));
        _1004->setFont(font2);
        _1005 = new QComboBox(DimensionPrefix_Dialog);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(250, 65, 81, 22));
        _1005->setFont(font2);
        _1005->raise();
        _1004->raise();
        _1000->raise();
        label_45->raise();
        label_53->raise();
        label_54->raise();
        _2->raise();
        _1->raise();
        label_56->raise();
        label_57->raise();
        label_58->raise();
        label_59->raise();
        _104->raise();
        _105->raise();
        groupBox->raise();
        _101->raise();
        _102->raise();
        _103->raise();
        _106->raise();
        _111->raise();
        _100->raise();

        retranslateUi(DimensionPrefix_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionPrefix_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionPrefix_Dialog)
    {
        DimensionPrefix_Dialog->setWindowTitle(QCoreApplication::translate("DimensionPrefix_Dialog", "Dimension tolerance text options", nullptr));
        label_45->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Text font", nullptr));
        label_53->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Height", nullptr));
        label_54->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Width", nullptr));
        _2->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "OK", nullptr));
        label_56->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Angle", nullptr));
        label_57->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Horiz just", nullptr));
        label_58->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Vert spacing", nullptr));
        label_59->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Vert just", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DimensionPrefix_Dialog", "Style", nullptr));
        _107->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Bold", nullptr));
        _109->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Italic", nullptr));
        _110->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Boxed", nullptr));
        _108->setText(QCoreApplication::translate("DimensionPrefix_Dialog", "Underline", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DimensionPrefix_Dialog: public Ui_DimensionPrefix_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONPREFIX_DIALOG_H
