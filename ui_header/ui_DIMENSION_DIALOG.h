/********************************************************************************
** Form generated from reading UI file 'DIMENSION_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSION_DIALOG_H
#define UI_DIMENSION_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/layerspinbox.h"
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Dimension_Dialog
{
public:
    QPushButton *_208;
    QLabel *label_44;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_50;
    QCheckBox *_220;
    QCheckBox *_225;
    QCheckBox *_226;
    QCheckBox *_219;
    QCheckBox *_217;
    QPushButton *_206;
    QPushButton *_207;
    QPushButton *_209;
    QPushButton *_215;
    QGroupBox *groupBox;
    QCheckBox *_203;
    QCheckBox *_224;
    QCheckBox *_204;
    QPushButton *_210;
    QLabel *label_51;
    RCDoubleSpinBox *_205;
    QGroupBox *groupBox_2;
    QLabel *label_45;
    QComboBox *_211;
    QLabel *label_52;
    QLabel *label_53;
    QCheckBox *_216;
    RCDoubleSpinBox *_212;
    RCDoubleSpinBox *_213;
    QComboBox *_2011;
    QPushButton *_1;
    QPushButton *_2;
    QPushButton *_223;
    QPushButton *_221;
    RCDoubleSpinBox *_200;
    RCDoubleSpinBox *_201;
    RCDoubleSpinBox *_218;
    RCDoubleSpinBox *_222;
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
    RCIntegerSpinBox *_202;
    QGroupBox *groupBox_4;
    QGraphicsView *_214;
    QGroupBox *groupBox_5;
    QPushButton *_230;
    QPushButton *_228;
    QPushButton *_229;
    QPushButton *_231;
    QPushButton *_232;
    QPushButton *_233;

    void setupUi(QDialog *Dimension_Dialog)
    {
        if (Dimension_Dialog->objectName().isEmpty())
            Dimension_Dialog->setObjectName(QString::fromUtf8("Dimension_Dialog"));
        Dimension_Dialog->resize(841, 425);
        Dimension_Dialog->setMinimumSize(QSize(841, 425));
        Dimension_Dialog->setMaximumSize(QSize(841, 425));
        QFont font;
        font.setPointSize(10);
        Dimension_Dialog->setFont(font);
        Dimension_Dialog->setLayoutDirection(Qt::LeftToRight);
        _208 = new QPushButton(Dimension_Dialog);
        _208->setObjectName(QString::fromUtf8("_208"));
        _208->setGeometry(QRect(346, 126, 100, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        _208->setFont(font1);
        _208->setAutoDefault(false);
        label_44 = new QLabel(Dimension_Dialog);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(20, 102, 77, 20));
        label_44->setFont(font1);
        label_44->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_46 = new QLabel(Dimension_Dialog);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(20, 130, 77, 20));
        label_46->setFont(font1);
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_47 = new QLabel(Dimension_Dialog);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(20, 154, 79, 20));
        label_47->setFont(font1);
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_48 = new QLabel(Dimension_Dialog);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(188, 128, 77, 20));
        label_48->setFont(font1);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_50 = new QLabel(Dimension_Dialog);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(186, 156, 81, 20));
        label_50->setFont(font1);
        label_50->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _220 = new QCheckBox(Dimension_Dialog);
        _220->setObjectName(QString::fromUtf8("_220"));
        _220->setGeometry(QRect(10, 184, 169, 20));
        _220->setFont(font1);
        _220->setLayoutDirection(Qt::RightToLeft);
        _220->setTristate(false);
        _225 = new QCheckBox(Dimension_Dialog);
        _225->setObjectName(QString::fromUtf8("_225"));
        _225->setGeometry(QRect(12, 202, 167, 20));
        _225->setFont(font1);
        _225->setLayoutDirection(Qt::RightToLeft);
        _225->setTristate(false);
        _226 = new QCheckBox(Dimension_Dialog);
        _226->setObjectName(QString::fromUtf8("_226"));
        _226->setGeometry(QRect(10, 220, 169, 20));
        _226->setFont(font1);
        _226->setLayoutDirection(Qt::RightToLeft);
        _226->setTristate(false);
        _219 = new QCheckBox(Dimension_Dialog);
        _219->setObjectName(QString::fromUtf8("_219"));
        _219->setGeometry(QRect(190, 88, 149, 21));
        _219->setFont(font1);
        _219->setLayoutDirection(Qt::RightToLeft);
        _219->setTristate(false);
        _217 = new QCheckBox(Dimension_Dialog);
        _217->setObjectName(QString::fromUtf8("_217"));
        _217->setGeometry(QRect(180, 106, 159, 21));
        _217->setFont(font1);
        _217->setLayoutDirection(Qt::RightToLeft);
        _217->setTristate(false);
        _206 = new QPushButton(Dimension_Dialog);
        _206->setObjectName(QString::fromUtf8("_206"));
        _206->setGeometry(QRect(346, 98, 100, 23));
        _206->setFont(font1);
        _206->setAutoDefault(false);
        _207 = new QPushButton(Dimension_Dialog);
        _207->setObjectName(QString::fromUtf8("_207"));
        _207->setGeometry(QRect(346, 156, 100, 23));
        _207->setFont(font1);
        _207->setAutoDefault(false);
        _209 = new QPushButton(Dimension_Dialog);
        _209->setObjectName(QString::fromUtf8("_209"));
        _209->setGeometry(QRect(346, 186, 100, 23));
        _209->setFont(font1);
        _209->setAutoDefault(false);
        _215 = new QPushButton(Dimension_Dialog);
        _215->setObjectName(QString::fromUtf8("_215"));
        _215->setGeometry(QRect(346, 216, 100, 23));
        _215->setFont(font1);
        _215->setAutoDefault(false);
        groupBox = new QGroupBox(Dimension_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 240, 217, 139));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        groupBox->setFont(font2);
        _203 = new QCheckBox(groupBox);
        _203->setObjectName(QString::fromUtf8("_203"));
        _203->setGeometry(QRect(2, 22, 201, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(10);
        font3.setBold(false);
        font3.setWeight(50);
        _203->setFont(font3);
        _203->setLayoutDirection(Qt::RightToLeft);
        _203->setTristate(false);
        _224 = new QCheckBox(groupBox);
        _224->setObjectName(QString::fromUtf8("_224"));
        _224->setGeometry(QRect(14, 40, 189, 21));
        _224->setFont(font3);
        _224->setLayoutDirection(Qt::RightToLeft);
        _224->setTristate(false);
        _204 = new QCheckBox(groupBox);
        _204->setObjectName(QString::fromUtf8("_204"));
        _204->setGeometry(QRect(10, 58, 193, 21));
        _204->setFont(font3);
        _204->setLayoutDirection(Qt::RightToLeft);
        _204->setTristate(false);
        _210 = new QPushButton(groupBox);
        _210->setObjectName(QString::fromUtf8("_210"));
        _210->setGeometry(QRect(60, 108, 103, 23));
        _210->setFont(font3);
        _210->setAutoDefault(false);
        label_51 = new QLabel(groupBox);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(34, 82, 93, 20));
        label_51->setFont(font3);
        label_51->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _205 = new RCDoubleSpinBox(groupBox);
        _205->setObjectName(QString::fromUtf8("_205"));
        _205->setGeometry(QRect(130, 80, 70, 24));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(false);
        font4.setWeight(50);
        _205->setFont(font4);
        _205->setMinimum(-100000000.000000000000000);
        _205->setMaximum(100000000.000000000000000);
        groupBox_2 = new QGroupBox(Dimension_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(231, 240, 215, 139));
        groupBox_2->setFont(font2);
        label_45 = new QLabel(groupBox_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(28, 24, 45, 20));
        label_45->setFont(font4);
        label_45->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _211 = new QComboBox(groupBox_2);
        _211->setObjectName(QString::fromUtf8("_211"));
        _211->setGeometry(QRect(70, 24, 111, 22));
        QFont font5;
        font5.setBold(false);
        font5.setWeight(50);
        _211->setFont(font5);
        label_52 = new QLabel(groupBox_2);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(26, 54, 73, 20));
        label_52->setFont(font4);
        label_52->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_53 = new QLabel(groupBox_2);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(26, 78, 71, 20));
        label_53->setFont(font4);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _216 = new QCheckBox(groupBox_2);
        _216->setObjectName(QString::fromUtf8("_216"));
        _216->setGeometry(QRect(58, 106, 119, 21));
        _216->setFont(font3);
        _216->setLayoutDirection(Qt::RightToLeft);
        _216->setTristate(false);
        _212 = new RCDoubleSpinBox(groupBox_2);
        _212->setObjectName(QString::fromUtf8("_212"));
        _212->setGeometry(QRect(108, 50, 70, 24));
        _212->setFont(font4);
        _212->setMinimum(-100000000.000000000000000);
        _212->setMaximum(100000000.000000000000000);
        _213 = new RCDoubleSpinBox(groupBox_2);
        _213->setObjectName(QString::fromUtf8("_213"));
        _213->setGeometry(QRect(108, 76, 70, 24));
        _213->setFont(font4);
        _213->setMinimum(-100000.000000000000000);
        _213->setMaximum(100000.000000000000000);
        _2011 = new QComboBox(groupBox_2);
        _2011->setObjectName(QString::fromUtf8("_2011"));
        _2011->setGeometry(QRect(80, 28, 111, 22));
        _2011->setFont(font5);
        _2011->raise();
        label_45->raise();
        _211->raise();
        label_52->raise();
        label_53->raise();
        _216->raise();
        _212->raise();
        _213->raise();
        _1 = new QPushButton(Dimension_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(636, 388, 75, 23));
        _1->setFont(font1);
        _2 = new QPushButton(Dimension_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(722, 388, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _223 = new QPushButton(Dimension_Dialog);
        _223->setObjectName(QString::fromUtf8("_223"));
        _223->setGeometry(QRect(278, 388, 89, 23));
        _223->setFont(font1);
        _223->setAutoDefault(false);
        _221 = new QPushButton(Dimension_Dialog);
        _221->setObjectName(QString::fromUtf8("_221"));
        _221->setGeometry(QRect(84, 388, 85, 23));
        _221->setFont(font1);
        _221->setAutoDefault(false);
        _200 = new RCDoubleSpinBox(Dimension_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setGeometry(QRect(106, 100, 70, 24));
        _200->setFont(font);
        _200->setMinimum(-100000000.000000000000000);
        _200->setMaximum(100000000.000000000000000);
        _201 = new RCDoubleSpinBox(Dimension_Dialog);
        _201->setObjectName(QString::fromUtf8("_201"));
        _201->setGeometry(QRect(106, 126, 70, 24));
        _201->setFont(font);
        _201->setMinimum(-100000000.000000000000000);
        _201->setMaximum(100000000.000000000000000);
        _218 = new RCDoubleSpinBox(Dimension_Dialog);
        _218->setObjectName(QString::fromUtf8("_218"));
        _218->setGeometry(QRect(266, 126, 70, 24));
        _218->setFont(font);
        _218->setDecimals(2);
        _218->setMinimum(0.000000000000000);
        _218->setMaximum(100000000.000000000000000);
        _218->setValue(0.000000000000000);
        _222 = new RCDoubleSpinBox(Dimension_Dialog);
        _222->setObjectName(QString::fromUtf8("_222"));
        _222->setGeometry(QRect(266, 154, 70, 24));
        _222->setFont(font);
        _222->setMinimum(-100000000.000000000000000);
        _222->setMaximum(100000000.000000000000000);
        groupBox_3 = new QGroupBox(Dimension_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(14, 4, 427, 79));
        QFont font6;
        font6.setPointSize(10);
        font6.setBold(true);
        font6.setWeight(75);
        groupBox_3->setFont(font6);
        _1002 = new QPushButton(groupBox_3);
        _1002->setObjectName(QString::fromUtf8("_1002"));
        _1002->setEnabled(true);
        _1002->setGeometry(QRect(144, 49, 24, 24));
        _1002->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-styles.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1002->setIcon(icon);
        _1002->setIconSize(QSize(20, 20));
        _1002->setAutoDefault(false);
        _1002->setFlat(true);
        label_103 = new QLabel(groupBox_3);
        label_103->setObjectName(QString::fromUtf8("label_103"));
        label_103->setGeometry(QRect(176, 52, 47, 20));
        label_103->setFont(font3);
        _1001 = new QPushButton(groupBox_3);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setEnabled(true);
        _1001->setGeometry(QRect(384, 24, 24, 24));
        _1001->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/PROPERTIES-layers.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1001->setIcon(icon1);
        _1001->setIconSize(QSize(20, 20));
        _1001->setAutoDefault(false);
        _1001->setFlat(true);
        _102 = new QSpinBox(groupBox_3);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(68, 50, 70, 22));
        _102->setFont(font3);
        _102->setMaximum(255);
        label_100 = new QLabel(groupBox_3);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(24, 25, 43, 20));
        label_100->setFont(font3);
        label_102 = new QLabel(groupBox_3);
        label_102->setObjectName(QString::fromUtf8("label_102"));
        label_102->setGeometry(QRect(24, 51, 45, 20));
        label_102->setFont(font3);
        _1000 = new QPushButton(groupBox_3);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setEnabled(true);
        _1000->setGeometry(QRect(144, 23, 24, 24));
        _1000->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/PROPERTIES-colours.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1000->setIcon(icon2);
        _1000->setIconSize(QSize(20, 20));
        _1000->setAutoDefault(false);
        _1000->setFlat(true);
        _100 = new QSpinBox(groupBox_3);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEnabled(true);
        _100->setGeometry(QRect(68, 24, 70, 22));
        _100->setFont(font3);
        _100->setMaximum(255);
        _103 = new WeightSpinBox(groupBox_3);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(228, 50, 70, 22));
        _103->setFont(font3);
        _103->setMinimum(0.100000000000000);
        _103->setMaximum(3.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _101 = new LayerSpinBox(groupBox_3);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setEnabled(true);
        _101->setGeometry(QRect(228, 24, 149, 22));
        _101->setFont(font3);
        _101->setMaximum(2049);
        _1003 = new QPushButton(groupBox_3);
        _1003->setObjectName(QString::fromUtf8("_1003"));
        _1003->setEnabled(true);
        _1003->setGeometry(QRect(306, 50, 24, 24));
        _1003->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1003->setIcon(icon3);
        _1003->setIconSize(QSize(20, 20));
        _1003->setAutoDefault(false);
        _1003->setFlat(true);
        label_101 = new QLabel(groupBox_3);
        label_101->setObjectName(QString::fromUtf8("label_101"));
        label_101->setGeometry(QRect(176, 26, 43, 20));
        label_101->setFont(font3);
        _202 = new RCIntegerSpinBox(Dimension_Dialog);
        _202->setObjectName(QString::fromUtf8("_202"));
        _202->setGeometry(QRect(106, 152, 69, 24));
        _202->setMaximum(100);
        groupBox_4 = new QGroupBox(Dimension_Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(454, 4, 379, 269));
        _214 = new QGraphicsView(groupBox_4);
        _214->setObjectName(QString::fromUtf8("_214"));
        _214->setGeometry(QRect(4, 28, 371, 229));
        _214->setMinimumSize(QSize(313, 159));
        _214->setMaximumSize(QSize(500, 500));
        _214->setFont(font);
        _214->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _214->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _214->setSceneRect(QRectF(0, 0, 313, 159));
        _214->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _214->setTransformationAnchor(QGraphicsView::NoAnchor);
        _214->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        groupBox_5 = new QGroupBox(Dimension_Dialog);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(454, 276, 379, 101));
        _230 = new QPushButton(groupBox_5);
        _230->setObjectName(QString::fromUtf8("_230"));
        _230->setGeometry(QRect(258, 30, 100, 23));
        _230->setFont(font1);
        _230->setAutoDefault(false);
        _228 = new QPushButton(groupBox_5);
        _228->setObjectName(QString::fromUtf8("_228"));
        _228->setGeometry(QRect(18, 30, 100, 23));
        _228->setFont(font1);
        _228->setAutoDefault(false);
        _229 = new QPushButton(groupBox_5);
        _229->setObjectName(QString::fromUtf8("_229"));
        _229->setGeometry(QRect(138, 30, 100, 23));
        _229->setFont(font1);
        _229->setAutoDefault(false);
        _231 = new QPushButton(groupBox_5);
        _231->setObjectName(QString::fromUtf8("_231"));
        _231->setGeometry(QRect(18, 62, 100, 23));
        _231->setFont(font1);
        _231->setAutoDefault(false);
        _232 = new QPushButton(groupBox_5);
        _232->setObjectName(QString::fromUtf8("_232"));
        _232->setGeometry(QRect(138, 60, 100, 23));
        _232->setFont(font1);
        _232->setAutoDefault(false);
        _233 = new QPushButton(groupBox_5);
        _233->setObjectName(QString::fromUtf8("_233"));
        _233->setGeometry(QRect(258, 58, 100, 23));
        _233->setFont(font1);
        _233->setAutoDefault(false);

        retranslateUi(Dimension_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Dimension_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dimension_Dialog)
    {
        Dimension_Dialog->setWindowTitle(QCoreApplication::translate("Dimension_Dialog", "Dimension options", nullptr));
        _208->setText(QCoreApplication::translate("Dimension_Dialog", "Prefix style", nullptr));
        label_44->setText(QCoreApplication::translate("Dimension_Dialog", "Extension", nullptr));
        label_46->setText(QCoreApplication::translate("Dimension_Dialog", "Gap", nullptr));
        label_47->setText(QCoreApplication::translate("Dimension_Dialog", "Dec places", nullptr));
        label_48->setText(QCoreApplication::translate("Dimension_Dialog", "Value scale", nullptr));
        label_50->setText(QCoreApplication::translate("Dimension_Dialog", "Text Y offset", nullptr));
        _220->setText(QCoreApplication::translate("Dimension_Dialog", "Fix dimension position", nullptr));
        _225->setText(QCoreApplication::translate("Dimension_Dialog", "Hide left line", nullptr));
        _226->setText(QCoreApplication::translate("Dimension_Dialog", "Hide right line", nullptr));
        _219->setText(QCoreApplication::translate("Dimension_Dialog", "Scale dimension", nullptr));
        _217->setText(QCoreApplication::translate("Dimension_Dialog", "Gap from dimension line", nullptr));
        _206->setText(QCoreApplication::translate("Dimension_Dialog", "Terminator", nullptr));
        _207->setText(QCoreApplication::translate("Dimension_Dialog", "Value style", nullptr));
        _209->setText(QCoreApplication::translate("Dimension_Dialog", "Suffix style", nullptr));
        _215->setText(QCoreApplication::translate("Dimension_Dialog", "Toler style", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Dimension_Dialog", "Dimension from a common line", nullptr));
        _203->setText(QCoreApplication::translate("Dimension_Dialog", "Dimension from a common line", nullptr));
        _224->setText(QCoreApplication::translate("Dimension_Dialog", "Reverse", nullptr));
        _204->setText(QCoreApplication::translate("Dimension_Dialog", "Stack common dimensions", nullptr));
        _210->setText(QCoreApplication::translate("Dimension_Dialog", "Terminator", nullptr));
        label_51->setText(QCoreApplication::translate("Dimension_Dialog", "Stack distance", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Dimension_Dialog", "Tolerance", nullptr));
        label_45->setText(QCoreApplication::translate("Dimension_Dialog", "Style", nullptr));
        label_52->setText(QCoreApplication::translate("Dimension_Dialog", "Upper", nullptr));
        label_53->setText(QCoreApplication::translate("Dimension_Dialog", "Lower", nullptr));
        _216->setText(QCoreApplication::translate("Dimension_Dialog", "Value text style", nullptr));
        _1->setText(QCoreApplication::translate("Dimension_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Dimension_Dialog", "Cancel", nullptr));
        _223->setText(QCoreApplication::translate("Dimension_Dialog", "Clone-all", nullptr));
        _221->setText(QCoreApplication::translate("Dimension_Dialog", "Clone", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Dimension_Dialog", "Defaults", nullptr));
#if QT_CONFIG(tooltip)
        _1002->setToolTip(QCoreApplication::translate("Dimension_Dialog", "Select Style", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1002->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1002->setText(QString());
        label_103->setText(QCoreApplication::translate("Dimension_Dialog", "Weight", nullptr));
#if QT_CONFIG(tooltip)
        _1001->setToolTip(QCoreApplication::translate("Dimension_Dialog", "Select Layer", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1001->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1001->setText(QString());
        label_100->setText(QCoreApplication::translate("Dimension_Dialog", "Colour", nullptr));
        label_102->setText(QCoreApplication::translate("Dimension_Dialog", "Style", nullptr));
#if QT_CONFIG(tooltip)
        _1000->setToolTip(QCoreApplication::translate("Dimension_Dialog", "Select Colour", nullptr));
#endif // QT_CONFIG(tooltip)
        _1000->setText(QString());
        _100->setSpecialValueText(QString());
        _103->setSpecialValueText(QString());
        _101->setSuffix(QString());
#if QT_CONFIG(tooltip)
        _1003->setToolTip(QCoreApplication::translate("Dimension_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1003->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1003->setText(QString());
        label_101->setText(QCoreApplication::translate("Dimension_Dialog", "Layer", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Dimension_Dialog", "Preview", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("Dimension_Dialog", "Functions", nullptr));
        _230->setText(QCoreApplication::translate("Dimension_Dialog", "Modify Value", nullptr));
        _228->setText(QCoreApplication::translate("Dimension_Dialog", "Modify Length", nullptr));
        _229->setText(QCoreApplication::translate("Dimension_Dialog", "Modify Equation", nullptr));
        _231->setText(QCoreApplication::translate("Dimension_Dialog", "Add Point", nullptr));
        _232->setText(QCoreApplication::translate("Dimension_Dialog", "Delete Point", nullptr));
        _233->setText(QCoreApplication::translate("Dimension_Dialog", "Move Point", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dimension_Dialog: public Ui_Dimension_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSION_DIALOG_H
