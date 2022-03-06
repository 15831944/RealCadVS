/********************************************************************************
** Form generated from reading UI file 'TEXT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXT_DIALOG_H
#define UI_TEXT_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Text_Dialog
{
public:
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_14;
    QGroupBox *groupBox;
    QCheckBox *_107;
    QCheckBox *_109;
    QCheckBox *_115;
    QCheckBox *_108;
    QCheckBox *_110;
    QCheckBox *_111;
    QCheckBox *_112;
    QCheckBox *_117;
    QPushButton *_1;
    QPushButton *_114;
    QPushButton *_118;
    QComboBox *_104;
    QComboBox *_105;
    QPushButton *_2;
    QGraphicsView *_113;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_102;
    RCDoubleSpinBox *_103;
    RCDoubleSpinBox *_106;
    QSpinBox *_116;
    QFontComboBox *_100;
    QPushButton *_2000;
    QComboBox *_1000;
    QPushButton *_1001;
    QPushButton *_1002;
    QPushButton *_1003;
    QPushButton *_1116;
    QPushButton *_1006;
    QPushButton *_1005;
    QPushButton *_1004;
    QPushButton *_1011;

    void setupUi(QDialog *Text_Dialog)
    {
        if (Text_Dialog->objectName().isEmpty())
            Text_Dialog->setObjectName(QString::fromUtf8("Text_Dialog"));
        Text_Dialog->resize(388, 485);
        Text_Dialog->setMinimumSize(QSize(388, 485));
        Text_Dialog->setMaximumSize(QSize(388, 485));
        QFont font;
        font.setPointSize(10);
        Text_Dialog->setFont(font);
        label_6 = new QLabel(Text_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(32, 11, 61, 20));
        label_6->setFont(font);
        label_7 = new QLabel(Text_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(46, 47, 50, 20));
        label_7->setFont(font);
        label_8 = new QLabel(Text_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(46, 74, 50, 20));
        label_8->setFont(font);
        label_9 = new QLabel(Text_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(46, 102, 50, 20));
        label_9->setFont(font);
        label_10 = new QLabel(Text_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(200, 74, 61, 20));
        label_10->setFont(font);
        label_11 = new QLabel(Text_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(200, 100, 71, 20));
        label_11->setFont(font);
        label_12 = new QLabel(Text_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(200, 47, 61, 20));
        label_12->setFont(font);
        label_14 = new QLabel(Text_Dialog);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(95, 160, 81, 20));
        label_14->setFont(font);
        groupBox = new QGroupBox(Text_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 180, 357, 101));
        groupBox->setFont(font);
        _107 = new QCheckBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(20, 25, 71, 20));
        _107->setFont(font);
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setAutoFillBackground(false);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);
        _109 = new QCheckBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(true);
        _109->setGeometry(QRect(20, 48, 71, 20));
        _109->setFont(font);
        _109->setLayoutDirection(Qt::RightToLeft);
        _109->setAutoFillBackground(false);
        _109->setCheckable(true);
        _109->setChecked(false);
        _109->setTristate(true);
        _115 = new QCheckBox(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setEnabled(true);
        _115->setGeometry(QRect(20, 71, 71, 20));
        _115->setFont(font);
        _115->setLayoutDirection(Qt::RightToLeft);
        _115->setAutoFillBackground(false);
        _115->setCheckable(true);
        _115->setChecked(false);
        _115->setTristate(true);
        _108 = new QCheckBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(true);
        _108->setGeometry(QRect(125, 25, 81, 20));
        _108->setFont(font);
        _108->setLayoutDirection(Qt::RightToLeft);
        _108->setAutoFillBackground(false);
        _108->setCheckable(true);
        _108->setChecked(false);
        _108->setTristate(true);
        _110 = new QCheckBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(true);
        _110->setGeometry(QRect(135, 48, 71, 20));
        _110->setFont(font);
        _110->setLayoutDirection(Qt::RightToLeft);
        _110->setAutoFillBackground(false);
        _110->setCheckable(true);
        _110->setChecked(false);
        _110->setTristate(true);
        _111 = new QCheckBox(groupBox);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(247, 25, 71, 20));
        _111->setFont(font);
        _111->setLayoutDirection(Qt::RightToLeft);
        _111->setAutoFillBackground(false);
        _111->setCheckable(true);
        _111->setChecked(false);
        _111->setTristate(true);
        _112 = new QCheckBox(groupBox);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(247, 48, 71, 20));
        _112->setFont(font);
        _112->setLayoutDirection(Qt::RightToLeft);
        _112->setAutoFillBackground(false);
        _112->setCheckable(true);
        _112->setChecked(false);
        _112->setTristate(true);
        _117 = new QCheckBox(Text_Dialog);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setEnabled(true);
        _117->setGeometry(QRect(102, 132, 141, 20));
        _117->setFont(font);
        _117->setLayoutDirection(Qt::RightToLeft);
        _117->setCheckable(true);
        _117->setChecked(false);
        _117->setTristate(true);
        _1 = new QPushButton(Text_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(34, 448, 75, 24));
        _1->setFont(font);
        _114 = new QPushButton(Text_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        _114->setGeometry(QRect(114, 448, 75, 24));
        _114->setFont(font);
        _118 = new QPushButton(Text_Dialog);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setEnabled(true);
        _118->setGeometry(QRect(194, 448, 75, 24));
        _118->setFont(font);
        _104 = new QComboBox(Text_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(270, 46, 73, 22));
        _105 = new QComboBox(Text_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(270, 72, 73, 22));
        _2 = new QPushButton(Text_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(276, 448, 75, 24));
        _2->setFont(font);
        _113 = new QGraphicsView(Text_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(39, 290, 307, 151));
        _113->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setInteractive(false);
        _113->setSceneRect(QRectF(0, 0, 300, 150));
        _113->setResizeAnchor(QGraphicsView::NoAnchor);
        _101 = new RCDoubleSpinBox(Text_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(88, 48, 73, 22));
        _101->setWrapping(false);
        _101->setAccelerated(true);
        _101->setMaximum(1000000000.000000000000000);
        _101->setValue(1.000000000000000);
        _102 = new RCDoubleSpinBox(Text_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(88, 74, 73, 22));
        _102->setWrapping(true);
        _102->setAccelerated(true);
        _102->setMinimum(-100000000.000000000000000);
        _102->setMaximum(100000000.000000000000000);
        _103 = new RCDoubleSpinBox(Text_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(88, 102, 73, 22));
        _103->setWrapping(true);
        _103->setAccelerated(true);
        _103->setMinimum(-360.000000000000000);
        _103->setMaximum(360.000000000000000);
        _106 = new RCDoubleSpinBox(Text_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(268, 100, 73, 22));
        _106->setWrapping(true);
        _106->setAccelerated(true);
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _106->setSingleStep(0.100000000000000);
        _116 = new QSpinBox(Text_Dialog);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(169, 158, 71, 22));
        _116->setAccelerated(true);
        _116->setMinimum(0);
        _116->setMaximum(255);
        _100 = new QFontComboBox(Text_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(88, 10, 255, 22));
        _100->setEditable(false);
        _100->setCurrentIndex(0);
        _100->setFontFilters(QFontComboBox::ProportionalFonts|QFontComboBox::ScalableFonts);
        _2000 = new QPushButton(Text_Dialog);
        _2000->setObjectName(QString::fromUtf8("_2000"));
        _2000->setGeometry(QRect(240, 157, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _2000->setIcon(icon);
        _2000->setIconSize(QSize(20, 20));
        _2000->setAutoDefault(false);
        _2000->setFlat(true);
        _1000 = new QComboBox(Text_Dialog);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setGeometry(QRect(88, 22, 255, 22));
        _1001 = new QPushButton(Text_Dialog);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setGeometry(QRect(164, 52, 15, 15));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/undo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(11, 11));
        _1001->setFlat(false);
        _1002 = new QPushButton(Text_Dialog);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setGeometry(QRect(164, 78, 15, 15));
        _1002->setIcon(icon1);
        _1002->setIconSize(QSize(11, 11));
        _1002->setFlat(false);
        _1003 = new QPushButton(Text_Dialog);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setGeometry(QRect(164, 106, 15, 15));
        _1003->setIcon(icon1);
        _1003->setIconSize(QSize(11, 11));
        _1003->setFlat(false);
        _1116 = new QPushButton(Text_Dialog);
        _1116->setObjectName(QString::fromUtf8("_1116"));
        _1116->setGeometry(QRect(264, 162, 15, 15));
        _1116->setIcon(icon1);
        _1116->setIconSize(QSize(11, 11));
        _1116->setFlat(false);
        _1006 = new QPushButton(Text_Dialog);
        _1006->setObjectName(QString::fromUtf8("_1006"));
        _1006->setGeometry(QRect(344, 104, 15, 15));
        _1006->setIcon(icon1);
        _1006->setIconSize(QSize(11, 11));
        _1006->setFlat(false);
        _1005 = new QPushButton(Text_Dialog);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(344, 76, 15, 15));
        _1005->setIcon(icon1);
        _1005->setIconSize(QSize(11, 11));
        _1005->setFlat(false);
        _1004 = new QPushButton(Text_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(344, 50, 15, 15));
        _1004->setIcon(icon1);
        _1004->setIconSize(QSize(11, 11));
        _1004->setFlat(false);
        _1011 = new QPushButton(Text_Dialog);
        _1011->setObjectName(QString::fromUtf8("_1011"));
        _1011->setGeometry(QRect(344, 14, 15, 15));
        _1011->setIcon(icon1);
        _1011->setIconSize(QSize(11, 11));
        _1011->setFlat(false);
        groupBox->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        label_12->raise();
        label_14->raise();
        _117->raise();
        _1->raise();
        _114->raise();
        _118->raise();
        _104->raise();
        _105->raise();
        _2->raise();
        _113->raise();
        _101->raise();
        _102->raise();
        _103->raise();
        _106->raise();
        _116->raise();
        _2000->raise();
        _1000->raise();
        _100->raise();
        _1001->raise();
        _1002->raise();
        _1003->raise();
        _1116->raise();
        _1006->raise();
        _1005->raise();
        _1004->raise();
        _1011->raise();
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _116);
        QWidget::setTabOrder(_116, _2000);
        QWidget::setTabOrder(_2000, _117);
        QWidget::setTabOrder(_117, _107);
        QWidget::setTabOrder(_107, _115);
        QWidget::setTabOrder(_115, _108);
        QWidget::setTabOrder(_108, _110);
        QWidget::setTabOrder(_110, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _1);
        QWidget::setTabOrder(_1, _114);
        QWidget::setTabOrder(_114, _118);
        QWidget::setTabOrder(_118, _2);
        QWidget::setTabOrder(_2, _113);
        QWidget::setTabOrder(_113, _109);

        retranslateUi(Text_Dialog);

        QMetaObject::connectSlotsByName(Text_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Text_Dialog)
    {
        Text_Dialog->setWindowTitle(QCoreApplication::translate("Text_Dialog", "Text Options", nullptr));
        label_6->setText(QCoreApplication::translate("Text_Dialog", "Text font", nullptr));
        label_7->setText(QCoreApplication::translate("Text_Dialog", "Height", nullptr));
        label_8->setText(QCoreApplication::translate("Text_Dialog", "Width", nullptr));
        label_9->setText(QCoreApplication::translate("Text_Dialog", "Angle", nullptr));
        label_10->setText(QCoreApplication::translate("Text_Dialog", "Vert just", nullptr));
        label_11->setText(QCoreApplication::translate("Text_Dialog", "Vert spacing", nullptr));
        label_12->setText(QCoreApplication::translate("Text_Dialog", "Horz just", nullptr));
        label_14->setText(QCoreApplication::translate("Text_Dialog", "Halo colour", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Text_Dialog", "Style", nullptr));
        _107->setText(QCoreApplication::translate("Text_Dialog", "Bold", nullptr));
        _109->setText(QCoreApplication::translate("Text_Dialog", "Italic", nullptr));
        _115->setText(QCoreApplication::translate("Text_Dialog", "Halo", nullptr));
        _108->setText(QCoreApplication::translate("Text_Dialog", "Underline", nullptr));
        _110->setText(QCoreApplication::translate("Text_Dialog", "Boxed", nullptr));
        _111->setText(QCoreApplication::translate("Text_Dialog", "Outline", nullptr));
        _112->setText(QCoreApplication::translate("Text_Dialog", "Scaled", nullptr));
        _117->setText(QCoreApplication::translate("Text_Dialog", "Text height in points", nullptr));
        _1->setText(QCoreApplication::translate("Text_Dialog", "OK", nullptr));
        _114->setText(QCoreApplication::translate("Text_Dialog", "Clone", nullptr));
        _118->setText(QCoreApplication::translate("Text_Dialog", "Clone-All", nullptr));
        _2->setText(QCoreApplication::translate("Text_Dialog", "Cancel", nullptr));
        _101->setSpecialValueText(QString());
#if QT_CONFIG(tooltip)
        _2000->setToolTip(QCoreApplication::translate("Text_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _2000->setText(QString());
        _1000->setCurrentText(QString());
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1001->setText(QString());
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1002->setText(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1003->setText(QString());
#if QT_CONFIG(tooltip)
        _1116->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1116->setText(QString());
#if QT_CONFIG(tooltip)
        _1006->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1006->setText(QString());
#if QT_CONFIG(tooltip)
        _1005->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1005->setText(QString());
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
#if QT_CONFIG(tooltip)
        _1011->setToolTip(QCoreApplication::translate("Text_Dialog", "Default", nullptr));
#endif // QT_CONFIG(tooltip)
        _1011->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Text_Dialog: public Ui_Text_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXT_DIALOG_H
