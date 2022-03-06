/********************************************************************************
** Form generated from reading UI file 'DEFINECOLOUR_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFINECOLOUR_DIALOG_H
#define UI_DEFINECOLOUR_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DefineColour_Dialog
{
public:
    QPushButton *_1;
    QLabel *label_42;
    QLabel *label_51;
    QLabel *label_52;
    QLabel *label_53;
    QLabel *label_54;
    QLabel *label_55;
    QLabel *label_56;
    QGraphicsView *_113;
    QSlider *_106;
    QSlider *_105;
    QSlider *_104;
    QSlider *_112;
    QSlider *_111;
    QSlider *_110;
    QSpinBox *_100;
    QDoubleSpinBox *_103;
    QDoubleSpinBox *_102;
    QDoubleSpinBox *_101;
    QGraphicsView *_115;
    QLabel *label;
    QLabel *label_2;
    QGraphicsView *_116;
    QTabWidget *_120;
    QWidget *HLS;
    QLabel *_121;
    QWidget *BW;
    QLabel *_122;
    QWidget *Palette;
    QLabel *_123;
    QDoubleSpinBox *_107;
    QDoubleSpinBox *_108;
    QDoubleSpinBox *_109;
    QPushButton *_114;
    QLabel *label_3;
    QPushButton *_2;

    void setupUi(QDialog *DefineColour_Dialog)
    {
        if (DefineColour_Dialog->objectName().isEmpty())
            DefineColour_Dialog->setObjectName(QString::fromUtf8("DefineColour_Dialog"));
        DefineColour_Dialog->resize(589, 460);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DefineColour_Dialog->sizePolicy().hasHeightForWidth());
        DefineColour_Dialog->setSizePolicy(sizePolicy);
        DefineColour_Dialog->setMinimumSize(QSize(589, 460));
        DefineColour_Dialog->setMaximumSize(QSize(589, 460));
        QFont font;
        font.setPointSize(8);
        DefineColour_Dialog->setFont(font);
        _1 = new QPushButton(DefineColour_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(290, 412, 75, 23));
        QFont font1;
        font1.setPointSize(9);
        _1->setFont(font1);
        _1->setAutoDefault(false);
        label_42 = new QLabel(DefineColour_Dialog);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(394, 20, 71, 20));
        QFont font2;
        font2.setPointSize(10);
        label_42->setFont(font2);
        label_42->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_51 = new QLabel(DefineColour_Dialog);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(22, 364, 51, 20));
        label_51->setFont(font2);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_52 = new QLabel(DefineColour_Dialog);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(22, 338, 51, 20));
        label_52->setFont(font2);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_53 = new QLabel(DefineColour_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(22, 312, 51, 20));
        label_53->setFont(font2);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_54 = new QLabel(DefineColour_Dialog);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(302, 338, 65, 20));
        label_54->setFont(font2);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_55 = new QLabel(DefineColour_Dialog);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(302, 364, 65, 20));
        label_55->setFont(font2);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_56 = new QLabel(DefineColour_Dialog);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(302, 312, 65, 20));
        label_56->setFont(font2);
        label_56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _113 = new QGraphicsView(DefineColour_Dialog);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(486, 144, 90, 154));
        _113->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _113->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _106 = new QSlider(DefineColour_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(146, 364, 128, 19));
        _106->setMaximum(255);
        _106->setOrientation(Qt::Horizontal);
        _105 = new QSlider(DefineColour_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(146, 338, 128, 19));
        _105->setMaximum(255);
        _105->setOrientation(Qt::Horizontal);
        _104 = new QSlider(DefineColour_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(146, 312, 128, 19));
        _104->setStyleSheet(QString::fromUtf8(""));
        _104->setMaximum(255);
        _104->setOrientation(Qt::Horizontal);
        _104->setInvertedAppearance(false);
        _104->setInvertedControls(false);
        _112 = new QSlider(DefineColour_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(446, 362, 128, 19));
        _112->setMaximum(255);
        _112->setTracking(true);
        _112->setOrientation(Qt::Horizontal);
        _111 = new QSlider(DefineColour_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(446, 336, 128, 19));
        _111->setMaximum(255);
        _111->setSingleStep(1);
        _111->setTracking(true);
        _111->setOrientation(Qt::Horizontal);
        _110 = new QSlider(DefineColour_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(446, 312, 128, 19));
        _110->setMaximum(255);
        _110->setTracking(true);
        _110->setOrientation(Qt::Horizontal);
        _100 = new QSpinBox(DefineColour_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(510, 18, 67, 22));
        _100->setFont(font2);
        _100->setFrame(true);
        _100->setMaximum(255);
        _103 = new QDoubleSpinBox(DefineColour_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(68, 362, 67, 22));
        _103->setFont(font2);
        _103->setFrame(true);
        _103->setAccelerated(false);
        _103->setKeyboardTracking(false);
        _103->setDecimals(2);
        _103->setMaximum(1.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _102 = new QDoubleSpinBox(DefineColour_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(68, 336, 67, 22));
        _102->setFont(font2);
        _102->setFrame(true);
        _102->setAccelerated(false);
        _102->setKeyboardTracking(false);
        _102->setDecimals(2);
        _102->setMaximum(1.000000000000000);
        _102->setSingleStep(0.100000000000000);
        _101 = new QDoubleSpinBox(DefineColour_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(68, 310, 67, 22));
        _101->setFont(font2);
        _101->setFrame(true);
        _101->setAccelerated(false);
        _101->setKeyboardTracking(false);
        _101->setDecimals(2);
        _101->setMaximum(1.000000000000000);
        _101->setSingleStep(0.100000000000000);
        _115 = new QGraphicsView(DefineColour_Dialog);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(392, 144, 90, 154));
        _115->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _115->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label = new QLabel(DefineColour_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(394, 124, 91, 16));
        label->setFont(font2);
        label_2 = new QLabel(DefineColour_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(488, 124, 91, 16));
        label_2->setFont(font2);
        _116 = new QGraphicsView(DefineColour_Dialog);
        _116->setObjectName(QString::fromUtf8("_116"));
        _116->setGeometry(QRect(392, 66, 183, 50));
        _116->setFrameShadow(QFrame::Sunken);
        _116->setLineWidth(1);
        _116->setMidLineWidth(0);
        _116->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _116->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _120 = new QTabWidget(DefineColour_Dialog);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(18, 10, 369, 289));
        _120->setTabPosition(QTabWidget::North);
        _120->setTabShape(QTabWidget::Rounded);
        _120->setIconSize(QSize(32, 16));
        _120->setUsesScrollButtons(false);
        HLS = new QWidget();
        HLS->setObjectName(QString::fromUtf8("HLS"));
        _121 = new QLabel(HLS);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(2, 2, 360, 256));
        _121->setFrameShape(QFrame::NoFrame);
        _121->setFrameShadow(QFrame::Plain);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/HLS.png"), QSize(), QIcon::Normal, QIcon::Off);
        _120->addTab(HLS, icon, QString());
        _120->setTabText(_120->indexOf(HLS), QString::fromUtf8(""));
        BW = new QWidget();
        BW->setObjectName(QString::fromUtf8("BW"));
        _122 = new QLabel(BW);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setGeometry(QRect(2, 2, 360, 256));
        _122->setFrameShape(QFrame::NoFrame);
        _122->setFrameShadow(QFrame::Plain);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/BandW.png"), QSize(), QIcon::Normal, QIcon::Off);
        _120->addTab(BW, icon1, QString());
        _120->setTabText(_120->indexOf(BW), QString::fromUtf8(""));
        Palette = new QWidget();
        Palette->setObjectName(QString::fromUtf8("Palette"));
        _123 = new QLabel(Palette);
        _123->setObjectName(QString::fromUtf8("_123"));
        _123->setGeometry(QRect(6, 2, 352, 256));
        _123->setFrameShape(QFrame::NoFrame);
        _123->setFrameShadow(QFrame::Plain);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/CPAL.png"), QSize(), QIcon::Normal, QIcon::Off);
        _120->addTab(Palette, icon2, QString());
        _107 = new QDoubleSpinBox(DefineColour_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(370, 310, 67, 22));
        _107->setFont(font2);
        _107->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        _107->setAccelerated(false);
        _107->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        _107->setKeyboardTracking(false);
        _107->setDecimals(4);
        _107->setMaximum(358.999900000000025);
        _107->setSingleStep(1.000000000000000);
        _108 = new QDoubleSpinBox(DefineColour_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(370, 336, 67, 22));
        _108->setFont(font2);
        _108->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        _108->setAccelerated(false);
        _108->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        _108->setKeyboardTracking(false);
        _108->setDecimals(4);
        _108->setMaximum(0.999900000000000);
        _108->setSingleStep(0.100000000000000);
        _109 = new QDoubleSpinBox(DefineColour_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(370, 362, 67, 22));
        _109->setFont(font2);
        _109->setAccelerated(false);
        _109->setKeyboardTracking(false);
        _109->setDecimals(4);
        _109->setMaximum(0.999900000000000);
        _109->setSingleStep(0.100000000000000);
        _114 = new QPushButton(DefineColour_Dialog);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(374, 412, 75, 23));
        _114->setFont(font1);
        _114->setAutoDefault(false);
        label_3 = new QLabel(DefineColour_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(394, 46, 97, 16));
        label_3->setFont(font2);
        _2 = new QPushButton(DefineColour_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(456, 412, 75, 23));
        _120->raise();
        _1->raise();
        label_42->raise();
        label_51->raise();
        label_52->raise();
        label_53->raise();
        label_54->raise();
        label_55->raise();
        label_56->raise();
        _113->raise();
        _106->raise();
        _105->raise();
        _104->raise();
        _112->raise();
        _111->raise();
        _110->raise();
        _100->raise();
        _103->raise();
        _102->raise();
        _101->raise();
        _115->raise();
        label->raise();
        label_2->raise();
        _116->raise();
        _107->raise();
        _108->raise();
        _109->raise();
        _114->raise();
        label_3->raise();
        _2->raise();
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _100);
        QWidget::setTabOrder(_100, _120);
        QWidget::setTabOrder(_120, _114);
        QWidget::setTabOrder(_114, _1);
        QWidget::setTabOrder(_1, _115);
        QWidget::setTabOrder(_115, _116);
        QWidget::setTabOrder(_116, _113);

        retranslateUi(DefineColour_Dialog);

        _1->setDefault(true);
        _120->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DefineColour_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DefineColour_Dialog)
    {
        DefineColour_Dialog->setWindowTitle(QCoreApplication::translate("DefineColour_Dialog", "Define colour", nullptr));
        _1->setText(QCoreApplication::translate("DefineColour_Dialog", "OK", nullptr));
        label_42->setText(QCoreApplication::translate("DefineColour_Dialog", "Index colour ", nullptr));
        label_51->setText(QCoreApplication::translate("DefineColour_Dialog", "Blue", nullptr));
        label_52->setText(QCoreApplication::translate("DefineColour_Dialog", "Green", nullptr));
        label_53->setText(QCoreApplication::translate("DefineColour_Dialog", "Red", nullptr));
        label_54->setText(QCoreApplication::translate("DefineColour_Dialog", "Lightness", nullptr));
        label_55->setText(QCoreApplication::translate("DefineColour_Dialog", "Saturation", nullptr));
        label_56->setText(QCoreApplication::translate("DefineColour_Dialog", "Hue", nullptr));
        label->setText(QCoreApplication::translate("DefineColour_Dialog", "New Colour", nullptr));
        label_2->setText(QCoreApplication::translate("DefineColour_Dialog", "Current Colour", nullptr));
        _121->setText(QString());
#if QT_CONFIG(tooltip)
        _120->setTabToolTip(_120->indexOf(HLS), QCoreApplication::translate("DefineColour_Dialog", "HLS", nullptr));
#endif // QT_CONFIG(tooltip)
        _122->setText(QString());
#if QT_CONFIG(tooltip)
        _120->setTabToolTip(_120->indexOf(BW), QCoreApplication::translate("DefineColour_Dialog", "Grey Scale", nullptr));
#endif // QT_CONFIG(tooltip)
        _123->setText(QString());
        _120->setTabText(_120->indexOf(Palette), QString());
#if QT_CONFIG(tooltip)
        _120->setTabToolTip(_120->indexOf(Palette), QCoreApplication::translate("DefineColour_Dialog", "Current Palette", nullptr));
#endif // QT_CONFIG(tooltip)
        _114->setText(QCoreApplication::translate("DefineColour_Dialog", "Apply", nullptr));
        label_3->setText(QCoreApplication::translate("DefineColour_Dialog", "Screen Preview", nullptr));
        _2->setText(QCoreApplication::translate("DefineColour_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DefineColour_Dialog: public Ui_DefineColour_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFINECOLOUR_DIALOG_H
