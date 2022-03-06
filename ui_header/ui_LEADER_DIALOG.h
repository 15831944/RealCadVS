/********************************************************************************
** Form generated from reading UI file 'LEADER_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEADER_DIALOG_H
#define UI_LEADER_DIALOG_H

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
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Leader_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QPushButton *_122;
    QPushButton *_123;
    QPushButton *_1004;
    QComboBox *_104;
    QGroupBox *groupBox;
    QComboBox *_117;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QCheckBox *_119;
    QLabel *label_14;
    RCDoubleSpinBox *_118;
    RCDoubleSpinBox *_120;
    WeightSpinBox *_124;
    QPushButton *_1005;
    QComboBox *_217;
    QLabel *label_3;
    QGroupBox *groupBox_4;
    QCheckBox *_110;
    QCheckBox *_111;
    QCheckBox *_112;
    QCheckBox *_114;
    QCheckBox *_115;
    QCheckBox *_116;
    QCheckBox *_121;
    RCDoubleSpinBox *_105;
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
    QLabel *label_7;
    RCDoubleSpinBox *_109;
    QLabel *label_6;
    QFontComboBox *_1033;
    RCDoubleSpinBox *_106;
    QLabel *label_9;
    QLabel *label_10;
    QComboBox *_108;
    RCDoubleSpinBox *_107;
    QLabel *label_8;
    QSpinBox *_125;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *_2033;
    QComboBox *_204;
    QComboBox *_208;
    QGroupBox *groupBox_2;
    QGraphicsView *_113;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *Leader_Dialog)
    {
        if (Leader_Dialog->objectName().isEmpty())
            Leader_Dialog->setObjectName(QString::fromUtf8("Leader_Dialog"));
        Leader_Dialog->resize(781, 396);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Leader_Dialog->sizePolicy().hasHeightForWidth());
        Leader_Dialog->setSizePolicy(sizePolicy);
        Leader_Dialog->setMinimumSize(QSize(781, 396));
        Leader_Dialog->setMaximumSize(QSize(781, 396));
        QFont font;
        font.setPointSize(10);
        Leader_Dialog->setFont(font);
        _2 = new QPushButton(Leader_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(666, 358, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Leader_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(580, 358, 75, 23));
        _1->setFont(font1);
        _122 = new QPushButton(Leader_Dialog);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setEnabled(true);
        _122->setGeometry(QRect(108, 358, 81, 23));
        _122->setFont(font1);
        _123 = new QPushButton(Leader_Dialog);
        _123->setObjectName(QString::fromUtf8("_123"));
        _123->setEnabled(true);
        _123->setGeometry(QRect(202, 358, 85, 23));
        _123->setFont(font1);
        _1004 = new QPushButton(Leader_Dialog);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(292, 220, 24, 24));
        _1004->setFont(font1);
        _1004->setMouseTracking(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1004->setIcon(icon);
        _1004->setIconSize(QSize(20, 20));
        _1004->setAutoDefault(false);
        _1004->setFlat(true);
        _104 = new QComboBox(Leader_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(292, 128, 81, 22));
        _104->setFont(font1);
        groupBox = new QGroupBox(Leader_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(398, 196, 375, 151));
        groupBox->setFont(font1);
        _117 = new QComboBox(groupBox);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(158, 26, 119, 22));
        _117->setFont(font1);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(76, 28, 77, 21));
        label_11->setFont(font1);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(76, 52, 81, 21));
        label_12->setFont(font1);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(74, 76, 81, 21));
        label_13->setFont(font1);
        _119 = new QCheckBox(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(68, 98, 199, 21));
        _119->setFont(font1);
        _119->setLayoutDirection(Qt::RightToLeft);
        _119->setCheckable(true);
        _119->setChecked(false);
        _119->setTristate(false);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(74, 122, 81, 21));
        label_14->setFont(font1);
        _118 = new RCDoubleSpinBox(groupBox);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(160, 50, 80, 24));
        _118->setFont(font1);
        _118->setMaximum(100000000.000000000000000);
        _120 = new RCDoubleSpinBox(groupBox);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(160, 76, 80, 24));
        _120->setFont(font1);
        _120->setMaximum(100000000.000000000000000);
        _124 = new WeightSpinBox(groupBox);
        _124->setObjectName(QString::fromUtf8("_124"));
        _124->setGeometry(QRect(160, 120, 80, 24));
        _124->setFont(font1);
        _124->setMinimum(0.100000000000000);
        _124->setMaximum(3.000000000000000);
        _124->setSingleStep(0.100000000000000);
        _1005 = new QPushButton(groupBox);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setGeometry(QRect(244, 122, 24, 24));
        _1005->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1005->setIcon(icon1);
        _1005->setIconSize(QSize(20, 20));
        _1005->setAutoDefault(false);
        _1005->setFlat(true);
        _217 = new QComboBox(groupBox);
        _217->setObjectName(QString::fromUtf8("_217"));
        _217->setEnabled(false);
        _217->setGeometry(QRect(172, 20, 119, 22));
        _217->setFont(font1);
        _217->raise();
        _117->raise();
        label_11->raise();
        label_12->raise();
        label_13->raise();
        _119->raise();
        label_14->raise();
        _118->raise();
        _120->raise();
        _124->raise();
        _1005->raise();
        label_3 = new QLabel(Leader_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(26, 132, 46, 21));
        label_3->setFont(font1);
        groupBox_4 = new QGroupBox(Leader_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 248, 377, 101));
        groupBox_4->setFont(font1);
        _110 = new QCheckBox(groupBox_4);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(48, 28, 83, 20));
        _110->setFont(font1);
        _110->setLayoutDirection(Qt::RightToLeft);
        _111 = new QCheckBox(groupBox_4);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(144, 28, 83, 20));
        _111->setFont(font1);
        _111->setLayoutDirection(Qt::RightToLeft);
        _112 = new QCheckBox(groupBox_4);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(242, 28, 69, 20));
        _112->setFont(font1);
        _112->setLayoutDirection(Qt::RightToLeft);
        _114 = new QCheckBox(groupBox_4);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(48, 47, 83, 20));
        _114->setFont(font1);
        _114->setLayoutDirection(Qt::RightToLeft);
        _115 = new QCheckBox(groupBox_4);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(144, 46, 83, 20));
        _115->setFont(font1);
        _115->setLayoutDirection(Qt::RightToLeft);
        _116 = new QCheckBox(groupBox_4);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(242, 46, 69, 20));
        _116->setFont(font1);
        _116->setLayoutDirection(Qt::RightToLeft);
        _121 = new QCheckBox(groupBox_4);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(48, 66, 83, 20));
        _121->setFont(font1);
        _121->setLayoutDirection(Qt::RightToLeft);
        _105 = new RCDoubleSpinBox(Leader_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(90, 128, 80, 24));
        _105->setFont(font1);
        _105->setMaximum(100000000.000000000000000);
        groupBox_3 = new QGroupBox(Leader_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(8, 6, 381, 79));
        groupBox_3->setFont(font1);
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(124, 49, 24, 24));
        _1002->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon2);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(154, 50, 41, 20));
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(346, 22, 24, 24));
        _1001->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon3);
        _1001->setIconSize(QSize(20, 20));
        _1001->setAutoDefault(false);
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(46, 50, 77, 22));
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(8, 25, 41, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(8, 51, 39, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(124, 23, 24, 24));
        _1000->setFont(font1);
        _1000->setIcon(icon);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(46, 24, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(196, 48, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(196, 22, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(274, 48, 24, 24));
        _1003->setFont(font1);
        _1003->setIcon(icon1);
        _1003->setIconSize(QSize(20, 20));
        _1003->setAutoDefault(false);
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(156, 24, 35, 20));
        label_101->setFont(font1);
        label_7 = new QLabel(Leader_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(26, 102, 59, 21));
        label_7->setFont(font1);
        _109 = new RCDoubleSpinBox(Leader_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(90, 180, 80, 24));
        _109->setFont(font1);
        _109->setMinimum(-360.000000000000000);
        _109->setMaximum(360.000000000000000);
        label_6 = new QLabel(Leader_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(212, 154, 67, 21));
        label_6->setFont(font1);
        _1033 = new QFontComboBox(Leader_Dialog);
        _1033->setObjectName(QString::fromUtf8("_1033"));
        _1033->setGeometry(QRect(90, 98, 283, 22));
        _1033->setFont(font1);
        _1033->setEditable(false);
        _1033->setMaxVisibleItems(20);
        _1033->setFrame(true);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        _1033->setCurrentFont(font2);
        _106 = new RCDoubleSpinBox(Leader_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(90, 154, 80, 24));
        _106->setFont(font1);
        _106->setMaximum(100000000.000000000000000);
        _106->setSingleStep(0.100000000000000);
        label_9 = new QLabel(Leader_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(212, 182, 71, 21));
        label_9->setFont(font1);
        label_10 = new QLabel(Leader_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(54, 222, 69, 21));
        label_10->setFont(font1);
        _108 = new QComboBox(Leader_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(292, 152, 81, 22));
        _108->setFont(font1);
        _107 = new RCDoubleSpinBox(Leader_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(292, 180, 81, 24));
        _107->setFont(font1);
        _107->setMinimum(-100000000.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _107->setSingleStep(0.100000000000000);
        label_8 = new QLabel(Leader_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(26, 184, 46, 21));
        label_8->setFont(font1);
        _125 = new QSpinBox(Leader_Dialog);
        _125->setObjectName(QString::fromUtf8("_125"));
        _125->setGeometry(QRect(126, 220, 63, 24));
        _125->setFont(font1);
        _125->setWrapping(true);
        _125->setMaximum(255);
        label_4 = new QLabel(Leader_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(212, 130, 69, 21));
        label_4->setFont(font1);
        label_5 = new QLabel(Leader_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(26, 156, 46, 21));
        label_5->setFont(font1);
        _2033 = new QComboBox(Leader_Dialog);
        _2033->setObjectName(QString::fromUtf8("_2033"));
        _2033->setEnabled(false);
        _2033->setGeometry(QRect(98, 93, 281, 26));
        _204 = new QComboBox(Leader_Dialog);
        _204->setObjectName(QString::fromUtf8("_204"));
        _204->setEnabled(false);
        _204->setGeometry(QRect(302, 125, 81, 22));
        _204->setFont(font1);
        _208 = new QComboBox(Leader_Dialog);
        _208->setObjectName(QString::fromUtf8("_208"));
        _208->setEnabled(false);
        _208->setGeometry(QRect(304, 155, 81, 22));
        _208->setFont(font1);
        groupBox_2 = new QGroupBox(Leader_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(396, 4, 377, 191));
        _113 = new QGraphicsView(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(4, 26, 369, 155));
        _113->setFont(font);
        _113->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setInteractive(false);
        _113->setSceneRect(QRectF(0, 0, 375, 141));
        _113->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        _113->setTransformationAnchor(QGraphicsView::NoAnchor);
        _113->setResizeAnchor(QGraphicsView::NoAnchor);
        _113->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        graphicsView = new QGraphicsView(Leader_Dialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(208, 222, 70, 20));
        _204->raise();
        _208->raise();
        _2033->raise();
        _2->raise();
        _1->raise();
        _122->raise();
        _123->raise();
        _1004->raise();
        _104->raise();
        groupBox->raise();
        label_3->raise();
        groupBox_4->raise();
        _105->raise();
        groupBox_3->raise();
        label_7->raise();
        _109->raise();
        label_6->raise();
        _1033->raise();
        _106->raise();
        label_9->raise();
        label_10->raise();
        _108->raise();
        _107->raise();
        label_8->raise();
        _125->raise();
        label_4->raise();
        label_5->raise();
        groupBox_2->raise();
        graphicsView->raise();
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _1033);
        QWidget::setTabOrder(_1033, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _109);
        QWidget::setTabOrder(_109, _104);
        QWidget::setTabOrder(_104, _108);
        QWidget::setTabOrder(_108, _107);
        QWidget::setTabOrder(_107, _125);
        QWidget::setTabOrder(_125, _1004);
        QWidget::setTabOrder(_1004, _110);
        QWidget::setTabOrder(_110, _114);
        QWidget::setTabOrder(_114, _121);
        QWidget::setTabOrder(_121, _111);
        QWidget::setTabOrder(_111, _115);
        QWidget::setTabOrder(_115, _112);
        QWidget::setTabOrder(_112, _116);
        QWidget::setTabOrder(_116, _117);
        QWidget::setTabOrder(_117, _118);
        QWidget::setTabOrder(_118, _120);
        QWidget::setTabOrder(_120, _119);
        QWidget::setTabOrder(_119, _124);
        QWidget::setTabOrder(_124, _1005);
        QWidget::setTabOrder(_1005, _122);
        QWidget::setTabOrder(_122, _123);
        QWidget::setTabOrder(_123, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _113);

        retranslateUi(Leader_Dialog);

        _1->setDefault(true);
        _2033->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(Leader_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Leader_Dialog)
    {
        Leader_Dialog->setWindowTitle(QCoreApplication::translate("Leader_Dialog", "Leader options", nullptr));
        _2->setText(QCoreApplication::translate("Leader_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Leader_Dialog", "OK", nullptr));
        _122->setText(QCoreApplication::translate("Leader_Dialog", "Clone", nullptr));
        _123->setText(QCoreApplication::translate("Leader_Dialog", "Clone-all", nullptr));
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
        groupBox->setTitle(QCoreApplication::translate("Leader_Dialog", "Terminator style", nullptr));
        label_11->setText(QCoreApplication::translate("Leader_Dialog", "Style", nullptr));
        label_12->setText(QCoreApplication::translate("Leader_Dialog", "Length", nullptr));
        label_13->setText(QCoreApplication::translate("Leader_Dialog", "Height", nullptr));
        _119->setText(QCoreApplication::translate("Leader_Dialog", "Specify terminator weight       ", nullptr));
        label_14->setText(QCoreApplication::translate("Leader_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1005->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1005->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1005->setText(QString());
        label_3->setText(QCoreApplication::translate("Leader_Dialog", "Height", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Leader_Dialog", "Text Style", nullptr));
        _110->setText(QCoreApplication::translate("Leader_Dialog", "Bold         ", nullptr));
        _111->setText(QCoreApplication::translate("Leader_Dialog", "Underline ", nullptr));
        _112->setText(QCoreApplication::translate("Leader_Dialog", "Outline ", nullptr));
        _114->setText(QCoreApplication::translate("Leader_Dialog", "Italic         ", nullptr));
        _115->setText(QCoreApplication::translate("Leader_Dialog", "Boxed      ", nullptr));
        _116->setText(QCoreApplication::translate("Leader_Dialog", "Halo     ", nullptr));
        _121->setText(QCoreApplication::translate("Leader_Dialog", "Scale text ", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Leader_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("Leader_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("Leader_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("Leader_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Leader_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("Leader_Dialog", "Layer", nullptr));
        label_7->setText(QCoreApplication::translate("Leader_Dialog", "Text font", nullptr));
        label_6->setText(QCoreApplication::translate("Leader_Dialog", "Vert just", nullptr));
        label_9->setText(QCoreApplication::translate("Leader_Dialog", "Vert spacing", nullptr));
        label_10->setText(QCoreApplication::translate("Leader_Dialog", "Halo colour", nullptr));
        label_8->setText(QCoreApplication::translate("Leader_Dialog", "Angle", nullptr));
        label_4->setText(QCoreApplication::translate("Leader_Dialog", "Horz just", nullptr));
        label_5->setText(QCoreApplication::translate("Leader_Dialog", "Width", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Leader_Dialog", "Preview", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Leader_Dialog: public Ui_Leader_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEADER_DIALOG_H
