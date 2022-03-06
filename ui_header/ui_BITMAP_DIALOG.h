/********************************************************************************
** Form generated from reading UI file 'BITMAP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITMAP_DIALOG_H
#define UI_BITMAP_DIALOG_H

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
#include <QtWidgets/QSlider>
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
    QPushButton *_1014;
    QGroupBox *groupBox_4;
    QCheckBox *_111;
    QLabel *label_36;
    QDoubleSpinBox *_117;
    QPushButton *_1006;
    QLabel *label_27;
    QLabel *label_28;
    QGraphicsView *_205;
    QPushButton *_1007;
    QDoubleSpinBox *_126;
    QSlider *_303;
    QPushButton *_1011;
    QPushButton *_1013;
    QPushButton *_1012;
    QLabel *previewback_label;
    QPushButton *_2;
    QPushButton *_1;
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
    QGroupBox *groupBox_3;
    QGraphicsView *_118;

    void setupUi(QDialog *Bitmap_Dialog)
    {
        if (Bitmap_Dialog->objectName().isEmpty())
            Bitmap_Dialog->setObjectName(QString::fromUtf8("Bitmap_Dialog"));
        Bitmap_Dialog->resize(934, 406);
        Bitmap_Dialog->setMinimumSize(QSize(934, 406));
        Bitmap_Dialog->setMaximumSize(QSize(934, 406));
        QFont font;
        font.setPointSize(8);
        Bitmap_Dialog->setFont(font);
        groupBox = new QGroupBox(Bitmap_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 86, 417, 275));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        _104 = new QCheckBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(98, 22, 151, 20));
        _104->setFont(font1);
        _104->setLayoutDirection(Qt::RightToLeft);
        _105 = new QCheckBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(98, 42, 151, 20));
        _105->setFont(font1);
        _105->setLayoutDirection(Qt::RightToLeft);
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(98, 116, 105, 20));
        label_22->setFont(font1);
        label_22->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(98, 146, 105, 20));
        label_23->setFont(font1);
        label_23->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_29 = new QLabel(groupBox);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(98, 174, 133, 20));
        label_29->setFont(font1);
        label_29->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(98, 206, 133, 20));
        label_30->setFont(font1);
        label_30->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_31 = new QLabel(groupBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(98, 236, 135, 20));
        label_31->setFont(font1);
        label_31->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(232, 115, 77, 24));
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
        _107->setGeometry(QRect(232, 144, 77, 24));
        _107->setFont(font2);
        _107->setDecimals(2);
        _107->setMinimum(0.000000000000000);
        _107->setMaximum(100000000.000000000000000);
        _107->setValue(10.000000000000000);
        _108 = new QComboBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(232, 172, 78, 26));
        _110 = new QComboBox(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(232, 232, 78, 26));
        _109 = new QComboBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(232, 202, 78, 26));
        _200 = new QLineEdit(groupBox);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setEnabled(false);
        _200->setGeometry(QRect(76, 64, 329, 20));
        _200->setReadOnly(true);
        _201 = new QLineEdit(groupBox);
        _201->setObjectName(QString::fromUtf8("_201"));
        _201->setEnabled(false);
        _201->setGeometry(QRect(76, 90, 329, 20));
        _201->setReadOnly(true);
        label_200 = new QLabel(groupBox);
        label_200->setObjectName(QString::fromUtf8("label_200"));
        label_200->setEnabled(true);
        label_200->setGeometry(QRect(14, 64, 53, 20));
        label_200->setFont(font1);
        label_200->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_201 = new QLabel(groupBox);
        label_201->setObjectName(QString::fromUtf8("label_201"));
        label_201->setEnabled(true);
        label_201->setGeometry(QRect(14, 90, 53, 20));
        label_201->setFont(font1);
        label_201->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1008 = new QComboBox(groupBox);
        _1008->setObjectName(QString::fromUtf8("_1008"));
        _1008->setGeometry(QRect(256, 172, 78, 26));
        _1009 = new QComboBox(groupBox);
        _1009->setObjectName(QString::fromUtf8("_1009"));
        _1009->setGeometry(QRect(256, 202, 78, 26));
        _1010 = new QComboBox(groupBox);
        _1010->setObjectName(QString::fromUtf8("_1010"));
        _1010->setGeometry(QRect(256, 232, 78, 26));
        _1014 = new QPushButton(groupBox);
        _1014->setObjectName(QString::fromUtf8("_1014"));
        _1014->setGeometry(QRect(210, 118, 19, 47));
        _1014->setMouseTracking(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/images/LinkIcon.png"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8(":/images/UnLinkIcon.png"), QSize(), QIcon::Disabled, QIcon::Off);
        _1014->setIcon(icon);
        _1014->setIconSize(QSize(20, 30));
        _1014->setCheckable(true);
        _1014->setChecked(true);
        _1014->setAutoDefault(false);
        _1014->setFlat(true);
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
        _1014->raise();
        groupBox_4 = new QGroupBox(Bitmap_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(444, 235, 477, 125));
        groupBox_4->setFont(font1);
        _111 = new QCheckBox(groupBox_4);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(14, 26, 167, 20));
        _111->setFont(font1);
        _111->setLayoutDirection(Qt::RightToLeft);
        label_36 = new QLabel(groupBox_4);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(108, 96, 53, 21));
        label_36->setFont(font1);
        label_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _117 = new QDoubleSpinBox(groupBox_4);
        _117->setObjectName(QString::fromUtf8("_117"));
        _117->setGeometry(QRect(166, 94, 77, 24));
        _117->setFont(font2);
        _117->setDecimals(6);
        _117->setMinimum(-0.000001000000000);
        _117->setMaximum(100.000000000000000);
        _117->setSingleStep(1.000000000000000);
        _117->setValue(0.000001000000000);
        _1006 = new QPushButton(groupBox_4);
        _1006->setObjectName(QString::fromUtf8("_1006"));
        _1006->setEnabled(true);
        _1006->setGeometry(QRect(414, 58, 24, 24));
        _1006->setMouseTracking(false);
        _1006->setText(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-clone-properties.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1006->setIcon(icon1);
        _1006->setIconSize(QSize(20, 20));
        _1006->setAutoDefault(false);
        _1006->setFlat(true);
        label_27 = new QLabel(groupBox_4);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(2, 59, 159, 20));
        label_27->setFont(font1);
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_28 = new QLabel(groupBox_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(254, 60, 81, 20));
        label_28->setFont(font1);
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        _205 = new QGraphicsView(groupBox_4);
        _205->setObjectName(QString::fromUtf8("_205"));
        _205->setGeometry(QRect(166, 60, 60, 20));
        _205->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _205->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _205->setSceneRect(QRectF(0, 0, 55, 20));
        _205->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        _205->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _1007 = new QPushButton(groupBox_4);
        _1007->setObjectName(QString::fromUtf8("_1007"));
        _1007->setGeometry(QRect(228, 59, 24, 24));
        _1007->setMouseTracking(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1007->setIcon(icon2);
        _1007->setIconSize(QSize(20, 20));
        _1007->setAutoDefault(false);
        _1007->setFlat(true);
        _126 = new QDoubleSpinBox(groupBox_4);
        _126->setObjectName(QString::fromUtf8("_126"));
        _126->setGeometry(QRect(338, 58, 71, 24));
        _126->setKeyboardTracking(false);
        _126->setDecimals(6);
        _126->setMinimum(-0.000001000000000);
        _126->setMaximum(1.000000000000000);
        _126->setSingleStep(0.100000000000000);
        _126->setValue(0.000000000000000);
        _303 = new QSlider(groupBox_4);
        _303->setObjectName(QString::fromUtf8("_303"));
        _303->setGeometry(QRect(264, 94, 165, 22));
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
        _303->setMinimum(0);
        _303->setMaximum(100);
        _303->setSingleStep(5);
        _303->setSliderPosition(100);
        _303->setOrientation(Qt::Horizontal);
        _303->setInvertedAppearance(true);
        _303->setInvertedControls(true);
        _303->setTickPosition(QSlider::TicksBothSides);
        _303->setTickInterval(5);
        _1011 = new QPushButton(groupBox_4);
        _1011->setObjectName(QString::fromUtf8("_1011"));
        _1011->setGeometry(QRect(354, 24, 25, 25));
        _1011->setMinimumSize(QSize(25, 25));
        _1011->setMaximumSize(QSize(25, 25));
        _1011->setBaseSize(QSize(20, 20));
        _1011->setFont(font2);
        _1011->setText(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/TranspBack1-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/images/TranspBack1.png"), QSize(), QIcon::Normal, QIcon::On);
        _1011->setIcon(icon3);
        _1011->setIconSize(QSize(21, 21));
        _1011->setCheckable(true);
        _1011->setChecked(true);
        _1011->setAutoExclusive(true);
        _1011->setAutoDefault(false);
        _1011->setFlat(true);
        _1013 = new QPushButton(groupBox_4);
        _1013->setObjectName(QString::fromUtf8("_1013"));
        _1013->setGeometry(QRect(412, 24, 25, 25));
        _1013->setMinimumSize(QSize(25, 25));
        _1013->setMaximumSize(QSize(25, 25));
        _1013->setBaseSize(QSize(20, 20));
        _1013->setFont(font2);
        _1013->setText(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/TranspBack3-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/images/TranspBack3.png"), QSize(), QIcon::Normal, QIcon::On);
        _1013->setIcon(icon4);
        _1013->setIconSize(QSize(21, 21));
        _1013->setCheckable(true);
        _1013->setAutoExclusive(true);
        _1013->setAutoDefault(false);
        _1013->setFlat(true);
        _1012 = new QPushButton(groupBox_4);
        _1012->setObjectName(QString::fromUtf8("_1012"));
        _1012->setGeometry(QRect(384, 24, 25, 25));
        _1012->setMinimumSize(QSize(25, 25));
        _1012->setMaximumSize(QSize(25, 25));
        _1012->setBaseSize(QSize(20, 20));
        _1012->setFont(font2);
        _1012->setText(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/TranspBack2-off.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon5.addFile(QString::fromUtf8(":/images/TranspBack2.png"), QSize(), QIcon::Normal, QIcon::On);
        _1012->setIcon(icon5);
        _1012->setIconSize(QSize(21, 21));
        _1012->setCheckable(true);
        _1012->setAutoExclusive(true);
        _1012->setAutoDefault(false);
        _1012->setFlat(true);
        previewback_label = new QLabel(groupBox_4);
        previewback_label->setObjectName(QString::fromUtf8("previewback_label"));
        previewback_label->setGeometry(QRect(234, 28, 115, 17));
        previewback_label->setFont(font1);
        _2 = new QPushButton(Bitmap_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(822, 368, 75, 24));
        _2->setFont(font2);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Bitmap_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(734, 368, 75, 24));
        _1->setFont(font2);
        _1->setAutoDefault(false);
        groupBox_2 = new QGroupBox(Bitmap_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(16, 4, 415, 82));
        _1002 = new QPushButton(groupBox_2);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(138, 50, 24, 24));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon6);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_2);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setEnabled(false);
        label_103->setGeometry(QRect(182, 52, 41, 20));
        label_103->setFont(font1);
        _1001 = new QPushButton(groupBox_2);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(382, 24, 24, 24));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon7);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_2);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(58, 52, 77, 22));
        _102->setFont(font2);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_2);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setEnabled(false);
        label_100->setGeometry(QRect(16, 27, 39, 20));
        label_100->setFont(font1);
        label_102 = new QLabel(groupBox_2);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setEnabled(false);
        label_102->setGeometry(QRect(16, 53, 41, 20));
        label_102->setFont(font1);
        _1000 = new QPushButton(groupBox_2);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(138, 25, 24, 24));
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_2);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(58, 26, 77, 22));
        _100->setFont(font2);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_2);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(224, 50, 77, 22));
        _103->setFont(font2);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_2);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(224, 24, 157, 22));
        _101->setFont(font2);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_2);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(304, 50, 24, 24));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon8);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_2);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setEnabled(false);
        label_101->setGeometry(QRect(184, 26, 37, 20));
        label_101->setFont(font1);
        _119 = new QPushButton(Bitmap_Dialog);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setEnabled(true);
        _119->setGeometry(QRect(130, 368, 75, 24));
        _119->setFont(font1);
        _120 = new QPushButton(Bitmap_Dialog);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setEnabled(true);
        _120->setGeometry(QRect(246, 368, 75, 24));
        _120->setFont(font1);
        groupBox_3 = new QGroupBox(Bitmap_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(444, 4, 477, 231));
        groupBox_3->setMinimumSize(QSize(459, 0));
        _118 = new QGraphicsView(groupBox_3);
        _118->setObjectName(QString::fromUtf8("_118"));
        _118->setGeometry(QRect(4, 26, 469, 197));
        _118->setMinimumSize(QSize(0, 0));
        _118->setMaximumSize(QSize(500, 500));
        _118->setAcceptDrops(false);
        _118->setFrameShape(QFrame::StyledPanel);
        _118->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _118->setInteractive(false);
        _118->setSceneRect(QRectF(0, 0, 485, 172));
        _118->setResizeAnchor(QGraphicsView::AnchorViewCenter);

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
#if QT_CONFIG(tooltip)
        _1014->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Link/Unlink", nullptr));
#endif // QT_CONFIG(tooltip)
        _1014->setText(QString());
        groupBox_4->setTitle(QCoreApplication::translate("Bitmap_Dialog", "Transparency options", nullptr));
        _111->setText(QCoreApplication::translate("Bitmap_Dialog", "Transparent bitmap", nullptr));
        label_36->setText(QCoreApplication::translate("Bitmap_Dialog", "Opacity", nullptr));
        _117->setSuffix(QCoreApplication::translate("Bitmap_Dialog", "%", nullptr));
#if QT_CONFIG(tooltip)
        _1006->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Colour and Range", nullptr));
#endif // QT_CONFIG(tooltip)
        label_27->setText(QCoreApplication::translate("Bitmap_Dialog", "Bitmap Transparent Colour", nullptr));
        label_28->setText(QCoreApplication::translate("Bitmap_Dialog", "Colour Range", nullptr));
#if QT_CONFIG(tooltip)
        _1007->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1007->setText(QString());
#if QT_CONFIG(tooltip)
        _1011->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1013->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        _1012->setToolTip(QCoreApplication::translate("Bitmap_Dialog", "Change preview background", nullptr));
#endif // QT_CONFIG(tooltip)
        previewback_label->setText(QCoreApplication::translate("Bitmap_Dialog", "Preview background", nullptr));
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
        groupBox_3->setTitle(QCoreApplication::translate("Bitmap_Dialog", "Preview", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Bitmap_Dialog: public Ui_Bitmap_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITMAP_DIALOG_H
