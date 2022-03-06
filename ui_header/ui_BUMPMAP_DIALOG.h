/********************************************************************************
** Form generated from reading UI file 'BUMPMAP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUMPMAP_DIALOG_H
#define UI_BUMPMAP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Bumpmap_Dialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *_100;
    QPushButton *_106;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_4;
    QDoubleSpinBox *_101;
    QLabel *label_3;
    QDoubleSpinBox *_102;
    QLabel *label_6;
    QDoubleSpinBox *_103;
    QLabel *label_7;
    QDoubleSpinBox *_104;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *_105;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *Bumpmap_Dialog)
    {
        if (Bumpmap_Dialog->objectName().isEmpty())
            Bumpmap_Dialog->setObjectName(QString::fromUtf8("Bumpmap_Dialog"));
        Bumpmap_Dialog->resize(490, 247);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Bumpmap_Dialog->sizePolicy().hasHeightForWidth());
        Bumpmap_Dialog->setSizePolicy(sizePolicy);
        Bumpmap_Dialog->setMinimumSize(QSize(490, 247));
        Bumpmap_Dialog->setMaximumSize(QSize(490, 247));
        QFont font;
        font.setPointSize(10);
        Bumpmap_Dialog->setFont(font);
        verticalLayout_2 = new QVBoxLayout(Bumpmap_Dialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(Bumpmap_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        _100 = new QLineEdit(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));

        horizontalLayout_2->addWidget(_100);

        _106 = new QPushButton(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));

        horizontalLayout_2->addWidget(_106);


        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(72, 16777215));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(71, 16777215));

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        _101 = new QDoubleSpinBox(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setMinimumSize(QSize(90, 0));
        _101->setDecimals(4);
        _101->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_101, 0, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(71, 16777215));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        _102 = new QDoubleSpinBox(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setMinimumSize(QSize(90, 0));
        _102->setDecimals(4);
        _102->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_102, 0, 4, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMaximumSize(QSize(71, 16777215));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        _103 = new QDoubleSpinBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setMinimumSize(QSize(90, 0));
        _103->setDecimals(4);
        _103->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_103, 1, 2, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(71, 16777215));

        gridLayout->addWidget(label_7, 1, 3, 1, 1);

        _104 = new QDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setMinimumSize(QSize(90, 0));
        _104->setDecimals(4);
        _104->setMaximum(1000000.000000000000000);

        gridLayout->addWidget(_104, 1, 4, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        _105 = new QCheckBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);

        horizontalLayout_3->addWidget(_105);

        horizontalSpacer_2 = new QSpacerItem(154, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalSpacer = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(Bumpmap_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(Bumpmap_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));

        horizontalLayout->addWidget(_2);


        verticalLayout_2->addLayout(horizontalLayout);

        QWidget::setTabOrder(_100, _106);
        QWidget::setTabOrder(_106, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Bumpmap_Dialog);

        QMetaObject::connectSlotsByName(Bumpmap_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Bumpmap_Dialog)
    {
        Bumpmap_Dialog->setWindowTitle(QCoreApplication::translate("Bumpmap_Dialog", "Bumpmap options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Bumpmap_Dialog", "Bumpmap parameters", nullptr));
        label_2->setText(QCoreApplication::translate("Bumpmap_Dialog", "Filename", nullptr));
        _106->setText(QCoreApplication::translate("Bumpmap_Dialog", "Browse", nullptr));
        label_5->setText(QCoreApplication::translate("Bumpmap_Dialog", "Area", nullptr));
        label_4->setText(QCoreApplication::translate("Bumpmap_Dialog", "X left", nullptr));
        label_3->setText(QCoreApplication::translate("Bumpmap_Dialog", "Y bottom", nullptr));
        label_6->setText(QCoreApplication::translate("Bumpmap_Dialog", "X right", nullptr));
        label_7->setText(QCoreApplication::translate("Bumpmap_Dialog", "Y top", nullptr));
        _105->setText(QCoreApplication::translate("Bumpmap_Dialog", "Swap X and Y bump map directions", nullptr));
        _1->setText(QCoreApplication::translate("Bumpmap_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("Bumpmap_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Bumpmap_Dialog: public Ui_Bumpmap_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUMPMAP_DIALOG_H
