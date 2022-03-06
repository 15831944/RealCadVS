/********************************************************************************
** Form generated from reading UI file 'FIGURE_DIALOG_v7.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGURE_DIALOG_V7_H
#define UI_FIGURE_DIALOG_V7_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
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

class Ui_Figure_Dialog
{
public:
    QPushButton *_1;
    QPushButton *_2;
    QPushButton *_125;
    QPushButton *_126;
    QPushButton *_127;
    QPushButton *_130;
    QPushButton *_129;
    QCheckBox *_124;
    QGroupBox *groupBox;
    QLineEdit *_119;
    QLineEdit *_120;
    QLabel *label_50;
    QLabel *label_55;
    QCheckBox *_115;
    QCheckBox *_114;
    QCheckBox *_112;
    QCheckBox *_113;
    QCheckBox *_111;
    QLineEdit *_121;
    QLabel *label_62;
    RCDoubleSpinBox *_108;
    RCDoubleSpinBox *_109;
    QLabel *label_59;
    QLabel *label_57;
    RCDoubleSpinBox *_107;
    QLabel *label_56;
    QLabel *label_58;
    QLabel *label_61;
    QLabel *label_60;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_104;
    RCDoubleSpinBox *_105;
    QLineEdit *_110;
    QLabel *label_53;
    QLineEdit *_1014;
    QLineEdit *_1015;
    QLineEdit *_1016;
    QLineEdit *_1018;
    QLineEdit *_1019;
    QLineEdit *_1017;
    QPushButton *_122;
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
    QGraphicsView *_123;
    QPushButton *_128;

    void setupUi(QDialog *Figure_Dialog)
    {
        if (Figure_Dialog->objectName().isEmpty())
            Figure_Dialog->setObjectName(QString::fromUtf8("Figure_Dialog"));
        Figure_Dialog->resize(423, 658);
        Figure_Dialog->setMinimumSize(QSize(423, 658));
        Figure_Dialog->setMaximumSize(QSize(423, 658));
        QFont font;
        font.setPointSize(10);
        Figure_Dialog->setFont(font);
        _1 = new QPushButton(Figure_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(224, 618, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _1->setFont(font1);
        _2 = new QPushButton(Figure_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(308, 618, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _125 = new QPushButton(Figure_Dialog);
        _125->setObjectName(QString::fromUtf8("_125"));
        _125->setEnabled(false);
        _125->setGeometry(QRect(6, 558, 139, 23));
        _125->setFont(font1);
        _125->setAutoDefault(false);
        _126 = new QPushButton(Figure_Dialog);
        _126->setObjectName(QString::fromUtf8("_126"));
        _126->setEnabled(false);
        _126->setGeometry(QRect(152, 558, 141, 23));
        _126->setFont(font1);
        _126->setAutoDefault(false);
        _127 = new QPushButton(Figure_Dialog);
        _127->setObjectName(QString::fromUtf8("_127"));
        _127->setEnabled(false);
        _127->setGeometry(QRect(296, 558, 120, 23));
        _127->setFont(font1);
        _127->setAutoDefault(false);
        _130 = new QPushButton(Figure_Dialog);
        _130->setObjectName(QString::fromUtf8("_130"));
        _130->setEnabled(true);
        _130->setGeometry(QRect(90, 618, 75, 23));
        _130->setFont(font1);
        _130->setAutoDefault(false);
        _129 = new QPushButton(Figure_Dialog);
        _129->setObjectName(QString::fromUtf8("_129"));
        _129->setEnabled(true);
        _129->setGeometry(QRect(14, 618, 75, 23));
        _129->setFont(font1);
        _129->setAutoDefault(false);
        _124 = new QCheckBox(Figure_Dialog);
        _124->setObjectName(QString::fromUtf8("_124"));
        _124->setEnabled(false);
        _124->setGeometry(QRect(156, 532, 105, 21));
        _124->setLayoutDirection(Qt::RightToLeft);
        _124->setCheckable(true);
        _124->setChecked(false);
        _124->setTristate(false);
        groupBox = new QGroupBox(Figure_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(12, 84, 399, 288));
        _119 = new QLineEdit(groupBox);
        _119->setObjectName(QString::fromUtf8("_119"));
        _119->setGeometry(QRect(82, 205, 309, 20));
        _119->setReadOnly(true);
        _120 = new QLineEdit(groupBox);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(82, 229, 309, 20));
        _120->setReadOnly(true);
        label_50 = new QLabel(groupBox);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(10, 205, 63, 20));
        label_50->setFont(font);
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_55 = new QLabel(groupBox);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(10, 229, 69, 20));
        label_55->setFont(font);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _115 = new QCheckBox(groupBox);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setEnabled(true);
        _115->setGeometry(QRect(88, 176, 183, 21));
        _115->setFont(font);
        _115->setLayoutDirection(Qt::RightToLeft);
        _115->setCheckable(true);
        _115->setChecked(false);
        _115->setTristate(false);
        _114 = new QCheckBox(groupBox);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setEnabled(true);
        _114->setGeometry(QRect(192, 150, 157, 21));
        _114->setFont(font);
        _114->setLayoutDirection(Qt::RightToLeft);
        _114->setCheckable(true);
        _114->setChecked(false);
        _114->setTristate(false);
        _112 = new QCheckBox(groupBox);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(20, 150, 149, 21));
        _112->setFont(font);
        _112->setLayoutDirection(Qt::RightToLeft);
        _112->setCheckable(true);
        _112->setChecked(false);
        _112->setTristate(false);
        _113 = new QCheckBox(groupBox);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setEnabled(true);
        _113->setGeometry(QRect(192, 130, 157, 21));
        _113->setFont(font);
        _113->setLayoutDirection(Qt::RightToLeft);
        _113->setCheckable(true);
        _113->setChecked(false);
        _113->setTristate(false);
        _111 = new QCheckBox(groupBox);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(22, 130, 147, 21));
        _111->setFont(font);
        _111->setLayoutDirection(Qt::RightToLeft);
        _111->setCheckable(true);
        _111->setChecked(false);
        _111->setTristate(false);
        _121 = new QLineEdit(groupBox);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(82, 254, 247, 20));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_121->sizePolicy().hasHeightForWidth());
        _121->setSizePolicy(sizePolicy);
        _121->setClearButtonEnabled(false);
        label_62 = new QLabel(groupBox);
        label_62->setObjectName(QString::fromUtf8("label_62"));
        label_62->setGeometry(QRect(8, 254, 71, 20));
        label_62->setFont(font);
        label_62->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _108 = new RCDoubleSpinBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(310, 51, 70, 24));
        _108->setMinimum(-360.000000000000000);
        _108->setMaximum(360.000000000000000);
        _109 = new RCDoubleSpinBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(310, 77, 70, 24));
        _109->setMinimum(-360.000000000000000);
        _109->setMaximum(360.000000000000000);
        label_59 = new QLabel(groupBox);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setGeometry(QRect(200, 55, 55, 20));
        label_59->setFont(font);
        label_59->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_57 = new QLabel(groupBox);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(20, 27, 47, 20));
        label_57->setFont(font);
        label_57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _107 = new RCDoubleSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(310, 25, 70, 24));
        _107->setMinimum(-360.000000000000000);
        _107->setMaximum(360.000000000000000);
        label_56 = new QLabel(groupBox);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(200, 29, 55, 20));
        label_56->setFont(font);
        label_56->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_58 = new QLabel(groupBox);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(20, 53, 47, 20));
        label_58->setFont(font);
        label_58->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_61 = new QLabel(groupBox);
        label_61->setObjectName(QString::fromUtf8("label_61"));
        label_61->setGeometry(QRect(20, 79, 47, 20));
        label_61->setFont(font);
        label_61->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_60 = new QLabel(groupBox);
        label_60->setObjectName(QString::fromUtf8("label_60"));
        label_60->setGeometry(QRect(200, 79, 55, 20));
        label_60->setFont(font);
        label_60->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(116, 77, 70, 24));
        _106->setMinimum(-100000000.000000000000000);
        _106->setMaximum(100000000.000000000000000);
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(116, 25, 70, 24));
        _104->setMinimum(-100000000.000000000000000);
        _104->setMaximum(100000000.000000000000000);
        _105 = new RCDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(116, 51, 70, 24));
        _105->setMinimum(-100000000.000000000000000);
        _105->setMaximum(100000000.000000000000000);
        _110 = new QLineEdit(groupBox);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(64, 105, 317, 21));
        _110->setReadOnly(true);
        label_53 = new QLabel(groupBox);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(20, 106, 39, 20));
        label_53->setFont(font);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1014 = new QLineEdit(groupBox);
        _1014->setObjectName(QString::fromUtf8("_1014"));
        _1014->setEnabled(false);
        _1014->setGeometry(QRect(64, 26, 51, 22));
        _1014->setReadOnly(true);
        _1015 = new QLineEdit(groupBox);
        _1015->setObjectName(QString::fromUtf8("_1015"));
        _1015->setEnabled(false);
        _1015->setGeometry(QRect(64, 52, 51, 22));
        _1015->setReadOnly(true);
        _1016 = new QLineEdit(groupBox);
        _1016->setObjectName(QString::fromUtf8("_1016"));
        _1016->setEnabled(false);
        _1016->setGeometry(QRect(64, 78, 51, 22));
        _1016->setReadOnly(true);
        _1018 = new QLineEdit(groupBox);
        _1018->setObjectName(QString::fromUtf8("_1018"));
        _1018->setEnabled(false);
        _1018->setGeometry(QRect(258, 52, 51, 22));
        _1018->setReadOnly(true);
        _1019 = new QLineEdit(groupBox);
        _1019->setObjectName(QString::fromUtf8("_1019"));
        _1019->setEnabled(false);
        _1019->setGeometry(QRect(258, 78, 51, 22));
        _1019->setReadOnly(true);
        _1017 = new QLineEdit(groupBox);
        _1017->setObjectName(QString::fromUtf8("_1017"));
        _1017->setEnabled(false);
        _1017->setGeometry(QRect(258, 26, 51, 22));
        _1017->setReadOnly(true);
        _122 = new QPushButton(groupBox);
        _122->setObjectName(QString::fromUtf8("_122"));
        _122->setGeometry(QRect(334, 252, 55, 21));
        sizePolicy.setHeightForWidth(_122->sizePolicy().hasHeightForWidth());
        _122->setSizePolicy(sizePolicy);
        _122->setMinimumSize(QSize(0, 0));
        _122->setMaximumSize(QSize(1000, 1000));
        _122->setFont(font1);
        _122->setFlat(false);
        groupBox_3 = new QGroupBox(Figure_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(12, 4, 397, 80));
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(false);
        _1002->setGeometry(QRect(134, 49, 24, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(170, 50, 37, 20));
        label_103->setFont(font);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(false);
        _1001->setGeometry(QRect(360, 24, 24, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(false);
        _102->setGeometry(QRect(54, 50, 77, 22));
        _102->setFont(font1);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(16, 27, 37, 20));
        label_100->setFont(font);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(16, 51, 37, 20));
        label_102->setFont(font);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(false);
        _1000->setGeometry(QRect(134, 25, 24, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(false);
        _100->setGeometry(QRect(54, 26, 77, 22));
        _100->setFont(font1);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(false);
        _103->setGeometry(QRect(208, 48, 77, 22));
        _103->setFont(font1);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(false);
        _101->setGeometry(QRect(208, 24, 149, 22));
        _101->setFont(font1);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(false);
        _1003->setGeometry(QRect(288, 48, 24, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(170, 26, 33, 20));
        label_101->setFont(font);
        _123 = new QGraphicsView(Figure_Dialog);
        _123->setObjectName(QString::fromUtf8("_123"));
        _123->setGeometry(QRect(10, 376, 403, 149));
        _123->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _123->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _128 = new QPushButton(Figure_Dialog);
        _128->setObjectName(QString::fromUtf8("_128"));
        _128->setEnabled(false);
        _128->setGeometry(QRect(136, 592, 171, 20));
        _128->setFont(font1);
        _128->setAutoDefault(false);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _102);
        QWidget::setTabOrder(_102, _1002);
        QWidget::setTabOrder(_1002, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _103);
        QWidget::setTabOrder(_103, _1003);
        QWidget::setTabOrder(_1003, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _113);
        QWidget::setTabOrder(_113, _114);
        QWidget::setTabOrder(_114, _115);
        QWidget::setTabOrder(_115, _121);
        QWidget::setTabOrder(_121, _125);
        QWidget::setTabOrder(_125, _124);
        QWidget::setTabOrder(_124, _126);
        QWidget::setTabOrder(_126, _127);
        QWidget::setTabOrder(_127, _128);
        QWidget::setTabOrder(_128, _129);
        QWidget::setTabOrder(_129, _130);
        QWidget::setTabOrder(_130, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _1019);
        QWidget::setTabOrder(_1019, _1017);
        QWidget::setTabOrder(_1017, _120);
        QWidget::setTabOrder(_120, _1015);
        QWidget::setTabOrder(_1015, _123);
        QWidget::setTabOrder(_123, _1018);
        QWidget::setTabOrder(_1018, _1016);
        QWidget::setTabOrder(_1016, _1014);
        QWidget::setTabOrder(_1014, _110);
        QWidget::setTabOrder(_110, _119);

        retranslateUi(Figure_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Figure_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Figure_Dialog)
    {
        Figure_Dialog->setWindowTitle(QCoreApplication::translate("Figure_Dialog", "Component options", nullptr));
        _1->setText(QCoreApplication::translate("Figure_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Figure_Dialog", "Cancel", nullptr));
        _125->setText(QCoreApplication::translate("Figure_Dialog", "Attach all components", nullptr));
        _126->setText(QCoreApplication::translate("Figure_Dialog", "Detach all components", nullptr));
        _127->setText(QCoreApplication::translate("Figure_Dialog", "Missing", nullptr));
        _130->setText(QCoreApplication::translate("Figure_Dialog", "Clone-all", nullptr));
        _129->setText(QCoreApplication::translate("Figure_Dialog", "Clone", nullptr));
        _124->setText(QCoreApplication::translate("Figure_Dialog", "Auto attach", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Figure_Dialog", "Settings", nullptr));
        label_50->setText(QCoreApplication::translate("Figure_Dialog", "Filename", nullptr));
        label_55->setText(QCoreApplication::translate("Figure_Dialog", "Alt. filename", nullptr));
        _115->setText(QCoreApplication::translate("Figure_Dialog", "Orient component to view", nullptr));
        _114->setText(QCoreApplication::translate("Figure_Dialog", "Use original weight", nullptr));
        _112->setText(QCoreApplication::translate("Figure_Dialog", "Use original colour", nullptr));
        _113->setText(QCoreApplication::translate("Figure_Dialog", "Use original style", nullptr));
        _111->setText(QCoreApplication::translate("Figure_Dialog", "Use original layer", nullptr));
        label_62->setText(QCoreApplication::translate("Figure_Dialog", "Default Path", nullptr));
        label_59->setText(QCoreApplication::translate("Figure_Dialog", "Y rotation", nullptr));
        label_57->setText(QCoreApplication::translate("Figure_Dialog", "X scale", nullptr));
        label_56->setText(QCoreApplication::translate("Figure_Dialog", "X rotation", nullptr));
        label_58->setText(QCoreApplication::translate("Figure_Dialog", "Y scale", nullptr));
        label_61->setText(QCoreApplication::translate("Figure_Dialog", "Z scale", nullptr));
        label_60->setText(QCoreApplication::translate("Figure_Dialog", "Z rotation", nullptr));
        label_53->setText(QCoreApplication::translate("Figure_Dialog", "Origin", nullptr));
        _1014->setText(QString());
        _1017->setText(QString());
        _122->setText(QCoreApplication::translate("Figure_Dialog", "Browse...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Figure_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Figure_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("Figure_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Figure_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("Figure_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("Figure_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("Figure_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Figure_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("Figure_Dialog", "Layer", nullptr));
        _128->setText(QCoreApplication::translate("Figure_Dialog", "Modify parametric component", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Figure_Dialog: public Ui_Figure_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGURE_DIALOG_V7_H
