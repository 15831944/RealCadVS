/********************************************************************************
** Form generated from reading UI file 'SURFACEPROPERTY_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SURFACEPROPERTY_DIALOG_H
#define UI_SURFACEPROPERTY_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SurfaceProperty_Dialog
{
public:
    QGroupBox *groupBox;
    QDoubleSpinBox *_108;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *_109;
    QLabel *label_9;
    QDoubleSpinBox *_106;
    QDoubleSpinBox *_107;
    QLabel *label_12;
    QDoubleSpinBox *_105;
    QLabel *label_8;
    QGroupBox *groupBox_2;
    QLabel *label_22;
    QDoubleSpinBox *_113;
    QDoubleSpinBox *_112;
    QLabel *label_20;
    QDoubleSpinBox *_111;
    QLabel *label_19;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout_3;
    QLabel *label_6;
    QComboBox *_100;
    QLabel *label_7;
    QDoubleSpinBox *_101;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QPushButton *_114;
    QPushButton *_115;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *_103;
    QCheckBox *_102;
    QSpacerItem *horizontalSpacer_3;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *SurfaceProperty_Dialog)
    {
        if (SurfaceProperty_Dialog->objectName().isEmpty())
            SurfaceProperty_Dialog->setObjectName(QString::fromUtf8("SurfaceProperty_Dialog"));
        SurfaceProperty_Dialog->resize(440, 369);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SurfaceProperty_Dialog->sizePolicy().hasHeightForWidth());
        SurfaceProperty_Dialog->setSizePolicy(sizePolicy);
        SurfaceProperty_Dialog->setMinimumSize(QSize(440, 369));
        SurfaceProperty_Dialog->setMaximumSize(QSize(440, 369));
        QFont font;
        font.setPointSize(10);
        SurfaceProperty_Dialog->setFont(font);
        groupBox = new QGroupBox(SurfaceProperty_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 125, 198, 185));
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _108 = new QDoubleSpinBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(86, 115, 90, 24));
        _108->setMinimumSize(QSize(90, 0));
        _108->setDecimals(4);
        _108->setMinimum(1.000000000000000);
        _108->setMaximum(1000.000000000000000);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(18, 118, 60, 16));
        label_10->setMinimumSize(QSize(60, 0));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(18, 144, 60, 16));
        label_11->setMinimumSize(QSize(60, 0));
        label_11->setFont(font);
        _109 = new QDoubleSpinBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(86, 142, 90, 24));
        _109->setMinimumSize(QSize(90, 0));
        _109->setDecimals(4);
        _109->setMaximum(1.000000000000000);
        _109->setSingleStep(0.100000000000000);
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(18, 66, 60, 16));
        label_9->setMinimumSize(QSize(60, 0));
        label_9->setFont(font);
        _106 = new QDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(86, 63, 90, 24));
        _106->setMinimumSize(QSize(90, 0));
        _106->setDecimals(4);
        _106->setMaximum(1.000000000000000);
        _106->setSingleStep(0.100000000000000);
        _107 = new QDoubleSpinBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(86, 89, 90, 24));
        _107->setMinimumSize(QSize(90, 0));
        _107->setDecimals(4);
        _107->setMaximum(1.000000000000000);
        _107->setSingleStep(0.100000000000000);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(18, 94, 60, 16));
        label_12->setMinimumSize(QSize(60, 0));
        label_12->setFont(font);
        _105 = new QDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(86, 36, 90, 24));
        _105->setMinimumSize(QSize(90, 0));
        _105->setDecimals(4);
        _105->setMaximum(1.000000000000000);
        _105->setSingleStep(0.100000000000000);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(18, 40, 60, 16));
        label_8->setMinimumSize(QSize(60, 0));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(SurfaceProperty_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(227, 125, 197, 185));
        groupBox_2->setMinimumSize(QSize(0, 0));
        groupBox_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(17, 90, 79, 16));
        label_22->setMinimumSize(QSize(70, 0));
        label_22->setFont(font);
        _113 = new QDoubleSpinBox(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(104, 87, 90, 24));
        _113->setMinimumSize(QSize(90, 0));
        _113->setDecimals(6);
        _113->setMinimum(-0.000010000000000);
        _113->setMaximum(10000.000000000000000);
        _113->setSingleStep(0.100000000000000);
        _113->setValue(0.000000000000000);
        _112 = new QDoubleSpinBox(groupBox_2);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(104, 61, 90, 24));
        _112->setMinimumSize(QSize(90, 0));
        _112->setDecimals(4);
        _112->setMaximum(1.000000000000000);
        _112->setSingleStep(0.100000000000000);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(17, 64, 79, 16));
        label_20->setMinimumSize(QSize(70, 0));
        label_20->setFont(font);
        _111 = new QDoubleSpinBox(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(104, 35, 90, 24));
        _111->setMinimumSize(QSize(90, 0));
        _111->setDecimals(4);
        _111->setMaximum(1.000000000000000);
        _111->setSingleStep(0.100000000000000);
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(17, 38, 79, 16));
        label_19->setMinimumSize(QSize(70, 0));
        label_19->setFont(font);
        label_19->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        layoutWidget = new QWidget(SurfaceProperty_Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(12, 16, 415, 68));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setContentsMargins(4, 7, 4, 2);
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(70, 0));
        label_6->setFont(font);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_6);

        _100 = new QComboBox(layoutWidget);
        _100->setObjectName(QString::fromUtf8("_100"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, _100);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setMinimumSize(QSize(70, 0));
        label_7->setFont(font);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_7);

        _101 = new QDoubleSpinBox(layoutWidget);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setMinimumSize(QSize(90, 0));
        _101->setWrapping(true);
        _101->setDecimals(4);
        _101->setMaximum(1.000000000000000);
        _101->setSingleStep(0.100000000000000);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, _101);


        horizontalLayout->addLayout(formLayout_3);

        horizontalSpacer_2 = new QSpacerItem(104, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        _114 = new QPushButton(layoutWidget);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setAutoDefault(false);

        verticalLayout_3->addWidget(_114);

        _115 = new QPushButton(layoutWidget);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setAutoDefault(false);

        verticalLayout_3->addWidget(_115);


        horizontalLayout->addLayout(verticalLayout_3);

        layoutWidget1 = new QWidget(SurfaceProperty_Dialog);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(12, 90, 415, 27));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        _103 = new QCheckBox(layoutWidget1);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);

        horizontalLayout_3->addWidget(_103);

        _102 = new QCheckBox(layoutWidget1);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);

        horizontalLayout_3->addWidget(_102);

        horizontalSpacer_3 = new QSpacerItem(6, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        layoutWidget2 = new QWidget(SurfaceProperty_Dialog);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(14, 326, 411, 32));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        _1 = new QPushButton(layoutWidget2);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout_4->addWidget(_1);

        _2 = new QPushButton(layoutWidget2);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setAutoDefault(false);

        horizontalLayout_4->addWidget(_2);

        QWidget::setTabOrder(_100, _114);
        QWidget::setTabOrder(_114, _101);
        QWidget::setTabOrder(_101, _115);
        QWidget::setTabOrder(_115, _103);
        QWidget::setTabOrder(_103, _102);
        QWidget::setTabOrder(_102, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _113);
        QWidget::setTabOrder(_113, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(SurfaceProperty_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SurfaceProperty_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SurfaceProperty_Dialog)
    {
        SurfaceProperty_Dialog->setWindowTitle(QCoreApplication::translate("SurfaceProperty_Dialog", "Surface property options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SurfaceProperty_Dialog", "Phong shading parameters", nullptr));
        label_10->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Exponent", nullptr));
        label_11->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Reflectivity", nullptr));
        label_9->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Diffuse", nullptr));
        label_12->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Specular", nullptr));
        label_8->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Ambient", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("SurfaceProperty_Dialog", "General shader parameters", nullptr));
        label_22->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Refractive index", nullptr));
        label_20->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Metallic", nullptr));
        label_19->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Smoothness", nullptr));
        label_6->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Shader", nullptr));
        label_7->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Transparency", nullptr));
        _114->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Texture map", nullptr));
        _115->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Bump map", nullptr));
        _103->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Generate reflections", nullptr));
        _102->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "White surface", nullptr));
        _1->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("SurfaceProperty_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SurfaceProperty_Dialog: public Ui_SurfaceProperty_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURFACEPROPERTY_DIALOG_H
