/********************************************************************************
** Form generated from reading UI file 'PLANEVERIFY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLANEVERIFY_DIALOG_H
#define UI_PLANEVERIFY_DIALOG_H

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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcplanesamplelabel.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_PlaneVerify_Dialog
{
public:
    QGroupBox *groupBox;
    QRadioButton *_114;
    QRadioButton *_116;
    QRadioButton *_117;
    QRadioButton *_119;
    QRadioButton *_118;
    QCheckBox *_120;
    QLineEdit *_108;
    QLabel *label_8;
    QComboBox *_121;
    QLabel *label_6;
    QLabel *label_23;
    QLabel *label_27;
    QPushButton *_1004;
    QPushButton *_1005;
    QDoubleSpinBox *_125;
    QDoubleSpinBox *_126;
    QPushButton *_1022;
    QPushButton *_110;
    QComboBox *_1021;
    QRadioButton *_115;
    QDoubleSpinBox *_134;
    QLabel *label_28;
    QGraphicsView *_205;
    QGraphicsView *_204;
    QSlider *_303;
    QPushButton *_1007;
    QGroupBox *groupBox_2;
    QLabel *_300;
    QLabel *_301;
    QLabel *label_30;
    QLabel *label_31;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_107;
    QGraphicsView *_220;
    RCPlaneSampleLabel *_1333;
    QPushButton *_1008;
    QLabel *_133;
    QPushButton *_1;
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
    QPushButton *_2;
    QGroupBox *groupBox_4;
    QGraphicsView *_219;
    QDoubleSpinBox *_130;
    QLabel *_302;
    QPushButton *_304;
    QCheckBox *_305;
    QPushButton *_1009;
    QPushButton *_1010;
    QPushButton *_1011;
    QPushButton *_131;
    QPushButton *_132;
    QPushButton *_113;
    QPushButton *_111;
    QPushButton *_112;

    void setupUi(QDialog *PlaneVerify_Dialog)
    {
        if (PlaneVerify_Dialog->objectName().isEmpty())
            PlaneVerify_Dialog->setObjectName(QString::fromUtf8("PlaneVerify_Dialog"));
        PlaneVerify_Dialog->resize(896, 379);
        PlaneVerify_Dialog->setMinimumSize(QSize(896, 379));
        PlaneVerify_Dialog->setMaximumSize(QSize(896, 379));
        QFont font;
        font.setPointSize(10);
        PlaneVerify_Dialog->setFont(font);
        groupBox = new QGroupBox(PlaneVerify_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(12, 104, 435, 223));
        _114 = new QRadioButton(groupBox);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(42, 25, 101, 18));
        _114->setFont(font);
        _114->setLayoutDirection(Qt::LeftToRight);
        _116 = new QRadioButton(groupBox);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(180, 26, 101, 18));
        _116->setFont(font);
        _116->setLayoutDirection(Qt::LeftToRight);
        _117 = new QRadioButton(groupBox);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(180, 47, 101, 18));
        _117->setFont(font);
        _117->setLayoutDirection(Qt::LeftToRight);
        _119 = new QRadioButton(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setGeometry(QRect(314, 47, 101, 18));
        _119->setFont(font);
        _119->setLayoutDirection(Qt::LeftToRight);
        _118 = new QRadioButton(groupBox);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(314, 26, 101, 18));
        _118->setFont(font);
        _118->setLayoutDirection(Qt::LeftToRight);
        _120 = new QCheckBox(groupBox);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setEnabled(true);
        _120->setGeometry(QRect(16, 68, 107, 19));
        _120->setFont(font);
#if QT_CONFIG(accessibility)
        _120->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        _120->setLayoutDirection(Qt::RightToLeft);
        _120->setCheckable(true);
        _120->setChecked(false);
        _120->setTristate(true);
        _108 = new QLineEdit(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(106, 92, 259, 20));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(6, 94, 93, 20));
        label_8->setFont(font);
        _121 = new QComboBox(groupBox);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(106, 124, 200, 22));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(4, 124, 99, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(6, 155, 97, 20));
        label_23->setFont(font);
        label_27 = new QLabel(groupBox);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(2, 188, 151, 20));
        label_27->setFont(font);
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _1004 = new QPushButton(groupBox);
        _1004->setObjectName(QString::fromUtf8("_1004"));
        _1004->setGeometry(QRect(400, 122, 24, 24));
        _1004->setMouseTracking(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1004->setIcon(icon);
        _1004->setIconSize(QSize(20, 20));
        _1004->setAutoDefault(false);
        _1004->setFlat(true);
        _1005 = new QPushButton(groupBox);
        _1005->setObjectName(QString::fromUtf8("_1005"));
        _1005->setEnabled(false);
        _1005->setGeometry(QRect(404, 187, 24, 24));
        _1005->setMouseTracking(false);
        _1005->setText(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-clone-properties.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1005->setIcon(icon1);
        _1005->setIconSize(QSize(20, 20));
        _1005->setAutoDefault(false);
        _1005->setFlat(true);
        _125 = new QDoubleSpinBox(groupBox);
        _125->setObjectName(QString::fromUtf8("_125"));
        _125->setGeometry(QRect(106, 153, 67, 24));
        _125->setKeyboardTracking(false);
        _125->setDecimals(6);
        _125->setMinimum(-0.000001000000000);
        _125->setMaximum(1.000000000000000);
        _125->setSingleStep(0.100000000000000);
        _125->setValue(0.000001000000000);
        _126 = new QDoubleSpinBox(groupBox);
        _126->setObjectName(QString::fromUtf8("_126"));
        _126->setGeometry(QRect(328, 187, 71, 24));
        _126->setKeyboardTracking(false);
        _126->setDecimals(6);
        _126->setMinimum(-0.000001000000000);
        _126->setMaximum(1.000000000000000);
        _126->setSingleStep(0.100000000000000);
        _126->setValue(0.000000000000000);
        _1022 = new QPushButton(groupBox);
        _1022->setObjectName(QString::fromUtf8("_1022"));
        _1022->setGeometry(QRect(372, 90, 24, 24));
        _1022->setMinimumSize(QSize(24, 24));
        _1022->setMaximumSize(QSize(1000, 1000));
        _1022->setBaseSize(QSize(20, 20));
        _1022->setFont(font);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/file.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1022->setIcon(icon2);
        _1022->setIconSize(QSize(20, 20));
        _1022->setAutoDefault(false);
        _1022->setFlat(true);
        _110 = new QPushButton(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(402, 90, 20, 20));
        _110->setMinimumSize(QSize(0, 0));
        _110->setMaximumSize(QSize(20, 20));
        _110->setBaseSize(QSize(20, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _110->setFont(font1);
        _110->setStyleSheet(QString::fromUtf8(""));
        _110->setText(QString::fromUtf8("..."));
        _110->setIconSize(QSize(16, 16));
        _110->setAutoDefault(false);
        _110->setFlat(true);
        _1021 = new QComboBox(groupBox);
        _1021->setObjectName(QString::fromUtf8("_1021"));
        _1021->setGeometry(QRect(108, 116, 200, 22));
        _115 = new QRadioButton(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(42, 45, 102, 20));
        QFont font2;
        font2.setPointSize(11);
        _115->setFont(font2);
        _134 = new QDoubleSpinBox(groupBox);
        _134->setObjectName(QString::fromUtf8("_134"));
        _134->setGeometry(QRect(178, 153, 69, 24));
        _134->setKeyboardTracking(false);
        _134->setDecimals(6);
        _134->setMinimum(-0.000001000000000);
        _134->setMaximum(100.000000000000000);
        _134->setSingleStep(1.000000000000000);
        _134->setValue(0.000001000000000);
        label_28 = new QLabel(groupBox);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(246, 189, 79, 20));
        label_28->setFont(font);
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _205 = new QGraphicsView(groupBox);
        _205->setObjectName(QString::fromUtf8("_205"));
        _205->setGeometry(QRect(158, 189, 60, 20));
        _205->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _205->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _205->setSceneRect(QRectF(0, 0, 55, 20));
        _205->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        _205->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _204 = new QGraphicsView(groupBox);
        _204->setObjectName(QString::fromUtf8("_204"));
        _204->setGeometry(QRect(320, 124, 60, 20));
        _204->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _204->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _204->setInteractive(true);
        _204->setSceneRect(QRectF(0, 0, 55, 20));
        _204->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _303 = new QSlider(groupBox);
        _303->setObjectName(QString::fromUtf8("_303"));
        _303->setGeometry(QRect(256, 155, 165, 22));
        _303->setStyleSheet(QString::fromUtf8("QSlider{\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 0));\n"
"border-radius: 11px; \n"
"}\n"
"QSlider::handle {\n"
"      background-color: white;\n"
"      border: 2px solid white;\n"
"      border-radius: 7px; \n"
"}\n"
"QSlider::handle:disabled{\n"
"      background-color: lightgrey;\n"
"      border: 2px solid lightgrey;\n"
"      border-radius: 7px; \n"
"}\n"
"QSlider:disabled{\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 100), stop:1 rgba(100, 100, 100, 0));\n"
"}\n"
"QSlider::add-page:disabled {\n"
"      background: lightgrey;\n"
"}\n"
"QSlider::sub-page:disabled {\n"
"      background: lightgrey;\n"
"}"));
        _303->setMaximum(100);
        _303->setSingleStep(5);
        _303->setOrientation(Qt::Horizontal);
        _303->setInvertedAppearance(false);
        _303->setInvertedControls(false);
        _303->setTickPosition(QSlider::TicksBothSides);
        _303->setTickInterval(5);
        _1007 = new QPushButton(groupBox);
        _1007->setObjectName(QString::fromUtf8("_1007"));
        _1007->setGeometry(QRect(220, 188, 24, 24));
        _1007->setMouseTracking(false);
        _1007->setIcon(icon);
        _1007->setIconSize(QSize(20, 20));
        _1007->setAutoDefault(false);
        _1007->setFlat(true);
        _1021->raise();
        _114->raise();
        _116->raise();
        _117->raise();
        _119->raise();
        _118->raise();
        _120->raise();
        _108->raise();
        label_8->raise();
        _121->raise();
        label_6->raise();
        label_23->raise();
        label_27->raise();
        _1004->raise();
        _1005->raise();
        _125->raise();
        _126->raise();
        _1022->raise();
        _110->raise();
        _115->raise();
        _134->raise();
        label_28->raise();
        _205->raise();
        _204->raise();
        _303->raise();
        _1007->raise();
        groupBox_2 = new QGroupBox(PlaneVerify_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(454, 16, 433, 135));
        _300 = new QLabel(groupBox_2);
        _300->setObjectName(QString::fromUtf8("_300"));
        _300->setGeometry(QRect(10, 27, 113, 20));
        _300->setFont(font);
        _301 = new QLabel(groupBox_2);
        _301->setObjectName(QString::fromUtf8("_301"));
        _301->setGeometry(QRect(10, 50, 113, 20));
        _301->setFont(font);
        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(10, 78, 121, 20));
        label_30->setFont(font);
        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(10, 102, 121, 20));
        label_31->setFont(font);
        _104 = new RCDoubleSpinBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(160, 24, 87, 24));
        _104->setKeyboardTracking(false);
        _104->setDecimals(6);
        _104->setMinimum(0.000001000000000);
        _104->setMaximum(1000000.000000000000000);
        _104->setSingleStep(1.000000000000000);
        _104->setValue(20.000000000000000);
        _105 = new RCDoubleSpinBox(groupBox_2);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(160, 50, 87, 24));
        _105->setKeyboardTracking(false);
        _105->setDecimals(6);
        _105->setMinimum(0.000001000000000);
        _105->setMaximum(1000000.000000000000000);
        _105->setSingleStep(1.000000000000000);
        _105->setValue(20.000000000000000);
        _106 = new RCDoubleSpinBox(groupBox_2);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(160, 76, 87, 24));
        _106->setKeyboardTracking(false);
        _106->setMinimum(-360.000000000000000);
        _106->setMaximum(360.000000000000000);
        _106->setSingleStep(1.000000000000000);
        _106->setValue(45.000000000000000);
        _107 = new RCDoubleSpinBox(groupBox_2);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(160, 102, 87, 24));
        _107->setKeyboardTracking(false);
        _107->setMinimum(-360.000000000000000);
        _107->setMaximum(360.000000000000000);
        _107->setSingleStep(1.000000000000000);
        _107->setValue(90.000000000000000);
        _220 = new QGraphicsView(groupBox_2);
        _220->setObjectName(QString::fromUtf8("_220"));
        _220->setGeometry(QRect(252, 26, 100, 100));
        _220->setMinimumSize(QSize(100, 100));
        _220->setMaximumSize(QSize(100, 100));
        _220->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _220->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _220->setInteractive(false);
        _220->setSceneRect(QRectF(0, 0, 100, 100));
        _220->setAlignment(Qt::AlignCenter);
        _220->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        _1333 = new RCPlaneSampleLabel(groupBox_2);
        _1333->setObjectName(QString::fromUtf8("_1333"));
        _1333->setGeometry(QRect(353, 31, 69, 90));
        _1333->setAutoFillBackground(false);
        _1008 = new QPushButton(groupBox_2);
        _1008->setObjectName(QString::fromUtf8("_1008"));
        _1008->setGeometry(QRect(136, 26, 19, 47));
        _1008->setMouseTracking(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/images/LinkIcon.png"), QSize(), QIcon::Normal, QIcon::On);
        icon3.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Disabled, QIcon::Off);
        _1008->setIcon(icon3);
        _1008->setIconSize(QSize(20, 30));
        _1008->setCheckable(true);
        _1008->setChecked(true);
        _1008->setAutoDefault(false);
        _1008->setFlat(true);
        _133 = new QLabel(groupBox_2);
        _133->setObjectName(QString::fromUtf8("_133"));
        _133->setGeometry(QRect(356, 64, 75, 23));
        _133->setFont(font);
        _1333->raise();
        _300->raise();
        _301->raise();
        label_30->raise();
        label_31->raise();
        _104->raise();
        _105->raise();
        _106->raise();
        _107->raise();
        _220->raise();
        _1008->raise();
        _133->raise();
        _1 = new QPushButton(PlaneVerify_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(704, 336, 75, 24));
        _1->setFont(font);
        groupBox_3 = new QGroupBox(PlaneVerify_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(14, 16, 433, 80));
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(146, 51, 24, 24));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon4);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(188, 52, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(382, 24, 24, 24));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon5);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(66, 52, 77, 22));
        _102->setFont(font1);
        _102->setKeyboardTracking(false);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(26, 27, 37, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(26, 53, 37, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(146, 25, 24, 24));
        _1000->setIcon(icon);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(66, 26, 77, 22));
        _100->setFont(font1);
        _100->setKeyboardTracking(false);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(230, 50, 77, 22));
        _103->setFont(font1);
        _103->setKeyboardTracking(false);
        _103->setMinimum(0.000000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(230, 24, 149, 22));
        _101->setFont(font1);
        _101->setKeyboardTracking(false);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(true);
        _1003->setGeometry(QRect(310, 50, 24, 24));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon6);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(188, 26, 33, 20));
        label_101->setFont(font);
        _2 = new QPushButton(PlaneVerify_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(796, 336, 79, 24));
        _2->setFont(font);
        _2->setAutoDefault(false);
        groupBox_4 = new QGroupBox(PlaneVerify_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(454, 156, 433, 170));
        _219 = new QGraphicsView(groupBox_4);
        _219->setObjectName(QString::fromUtf8("_219"));
        _219->setGeometry(QRect(4, 28, 330, 130));
        _219->setMinimumSize(QSize(330, 130));
        _219->setMaximumSize(QSize(330, 130));
        _219->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _219->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _219->setInteractive(false);
        _219->setSceneRect(QRectF(0, 0, 300, 100));
        _219->setAlignment(Qt::AlignCenter);
        _219->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        _130 = new QDoubleSpinBox(groupBox_4);
        _130->setObjectName(QString::fromUtf8("_130"));
        _130->setGeometry(QRect(340, 40, 87, 24));
        _130->setAccelerated(false);
        _130->setKeyboardTracking(false);
        _130->setMinimum(0.000000000000000);
        _130->setMaximum(1000000.000000000000000);
        _130->setSingleStep(5.000000000000000);
        _130->setValue(1000.000000000000000);
        _302 = new QLabel(groupBox_4);
        _302->setObjectName(QString::fromUtf8("_302"));
        _302->setEnabled(true);
        _302->setGeometry(QRect(344, 22, 71, 19));
        _302->setFont(font);
        _302->setAlignment(Qt::AlignCenter);
        _304 = new QPushButton(groupBox_4);
        _304->setObjectName(QString::fromUtf8("_304"));
        _304->setGeometry(QRect(344, 68, 79, 25));
        _304->setFont(font);
        _305 = new QCheckBox(groupBox_4);
        _305->setObjectName(QString::fromUtf8("_305"));
        _305->setGeometry(QRect(336, 140, 97, 20));
        _305->setFont(font);
        _1009 = new QPushButton(groupBox_4);
        _1009->setObjectName(QString::fromUtf8("_1009"));
        _1009->setGeometry(QRect(342, 106, 25, 25));
        _1009->setMinimumSize(QSize(25, 25));
        _1009->setMaximumSize(QSize(25, 25));
        _1009->setBaseSize(QSize(20, 20));
        _1009->setFont(font1);
        _1009->setText(QString::fromUtf8(""));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/TranspBack1-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon7.addFile(QString::fromUtf8(":/images/TranspBack1.png"), QSize(), QIcon::Normal, QIcon::On);
        _1009->setIcon(icon7);
        _1009->setIconSize(QSize(21, 21));
        _1009->setCheckable(true);
        _1009->setChecked(true);
        _1009->setAutoExclusive(true);
        _1009->setAutoDefault(false);
        _1009->setFlat(true);
        _1010 = new QPushButton(groupBox_4);
        _1010->setObjectName(QString::fromUtf8("_1010"));
        _1010->setGeometry(QRect(372, 106, 25, 25));
        _1010->setMinimumSize(QSize(25, 25));
        _1010->setMaximumSize(QSize(25, 25));
        _1010->setBaseSize(QSize(20, 20));
        _1010->setFont(font1);
        _1010->setText(QString::fromUtf8(""));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/TranspBack2-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon8.addFile(QString::fromUtf8(":/images/TranspBack2.png"), QSize(), QIcon::Normal, QIcon::On);
        _1010->setIcon(icon8);
        _1010->setIconSize(QSize(21, 21));
        _1010->setCheckable(true);
        _1010->setAutoExclusive(true);
        _1010->setAutoDefault(false);
        _1010->setFlat(true);
        _1011 = new QPushButton(groupBox_4);
        _1011->setObjectName(QString::fromUtf8("_1011"));
        _1011->setGeometry(QRect(400, 106, 25, 25));
        _1011->setMinimumSize(QSize(25, 25));
        _1011->setMaximumSize(QSize(25, 25));
        _1011->setBaseSize(QSize(20, 20));
        _1011->setFont(font1);
        _1011->setText(QString::fromUtf8(""));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/TranspBack3-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon9.addFile(QString::fromUtf8(":/images/TranspBack3.png"), QSize(), QIcon::Normal, QIcon::On);
        _1011->setIcon(icon9);
        _1011->setIconSize(QSize(21, 21));
        _1011->setCheckable(true);
        _1011->setAutoExclusive(true);
        _1011->setAutoDefault(false);
        _1011->setFlat(true);
        _131 = new QPushButton(PlaneVerify_Dialog);
        _131->setObjectName(QString::fromUtf8("_131"));
        _131->setEnabled(true);
        _131->setGeometry(QRect(184, 338, 75, 24));
        _131->setFont(font);
        _131->setAutoDefault(false);
        _132 = new QPushButton(PlaneVerify_Dialog);
        _132->setObjectName(QString::fromUtf8("_132"));
        _132->setEnabled(true);
        _132->setGeometry(QRect(284, 338, 75, 24));
        _132->setFont(font);
        _132->setAutoDefault(false);
        _113 = new QPushButton(PlaneVerify_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(544, 336, 79, 24));
        _113->setFont(font);
        _113->setAutoDefault(false);
        _111 = new QPushButton(PlaneVerify_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(34, 338, 125, 24));
        _111->setFont(font);
        _111->setAutoDefault(false);
        _112 = new QPushButton(PlaneVerify_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(458, 336, 75, 24));
        _112->setFont(font);
        _112->setAutoDefault(false);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _114);
        QWidget::setTabOrder(_114, _116);
        QWidget::setTabOrder(_116, _118);
        QWidget::setTabOrder(_118, _115);
        QWidget::setTabOrder(_115, _117);
        QWidget::setTabOrder(_117, _119);
        QWidget::setTabOrder(_119, _120);
        QWidget::setTabOrder(_120, _1022);
        QWidget::setTabOrder(_1022, _110);
        QWidget::setTabOrder(_110, _121);
        QWidget::setTabOrder(_121, _204);
        QWidget::setTabOrder(_204, _1004);
        QWidget::setTabOrder(_1004, _125);
        QWidget::setTabOrder(_125, _134);
        QWidget::setTabOrder(_134, _303);
        QWidget::setTabOrder(_303, _205);
        QWidget::setTabOrder(_205, _126);
        QWidget::setTabOrder(_126, _1005);
        QWidget::setTabOrder(_1005, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _130);
        QWidget::setTabOrder(_130, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _113);
        QWidget::setTabOrder(_113, _1);
        QWidget::setTabOrder(_1, _131);
        QWidget::setTabOrder(_131, _132);
        QWidget::setTabOrder(_132, _2);
        QWidget::setTabOrder(_2, _220);
        QWidget::setTabOrder(_220, _1021);
        QWidget::setTabOrder(_1021, _219);
        QWidget::setTabOrder(_219, _108);

        retranslateUi(PlaneVerify_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PlaneVerify_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PlaneVerify_Dialog)
    {
        PlaneVerify_Dialog->setWindowTitle(QCoreApplication::translate("PlaneVerify_Dialog", "Plane Options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PlaneVerify_Dialog", "Fill Options", nullptr));
        _114->setText(QCoreApplication::translate("PlaneVerify_Dialog", "No fill", nullptr));
        _116->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Pattern fill", nullptr));
        _117->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Solid fill", nullptr));
        _119->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Gradient fill", nullptr));
        _118->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Bitmap fill", nullptr));
        _120->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Display edges ", nullptr));
        label_8->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Pattern file", nullptr));
        label_6->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Gradient fill style", nullptr));
        label_23->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Transparency", nullptr));
        label_27->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Bitmap Transparent Colour", nullptr));
#if QT_CONFIG(tooltip)
        _1004->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1004->setText(QString());
#if QT_CONFIG(tooltip)
        _1005->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Colour and Range", nullptr));
#endif // QT_CONFIG(tooltip)
        _1022->setText(QString());
        _115->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Hatching", nullptr));
        _134->setSuffix(QCoreApplication::translate("PlaneVerify_Dialog", "%", nullptr));
        label_28->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Colour Range", nullptr));
#if QT_CONFIG(tooltip)
        _1007->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1007->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("PlaneVerify_Dialog", "Sizing", nullptr));
        _300->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Spacing", nullptr));
        _301->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Cross hatch spacing", nullptr));
        label_30->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Angle", nullptr));
        label_31->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Cross hatch angle", nullptr));
        _1333->setText(QString());
#if QT_CONFIG(tooltip)
        _1008->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Link/Unlink", nullptr));
#endif // QT_CONFIG(tooltip)
        _1008->setText(QString());
        _133->setText(QCoreApplication::translate("PlaneVerify_Dialog", "TextLabel", nullptr));
        _1->setText(QCoreApplication::translate("PlaneVerify_Dialog", "OK", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("PlaneVerify_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Layer", nullptr));
        _2->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Cancel", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("PlaneVerify_Dialog", "Preview", nullptr));
        _302->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Zoom", nullptr));
        _304->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Reset", nullptr));
        _305->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Show Preview", nullptr));
#if QT_CONFIG(tooltip)
        _1009->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1010->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1011->setToolTip(QCoreApplication::translate("PlaneVerify_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
        _131->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Clone", nullptr));
        _132->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Clone All", nullptr));
        _113->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Set Anchor", nullptr));
        _111->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Render Properties", nullptr));
        _112->setText(QCoreApplication::translate("PlaneVerify_Dialog", "Align", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaneVerify_Dialog: public Ui_PlaneVerify_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLANEVERIFY_DIALOG_H
