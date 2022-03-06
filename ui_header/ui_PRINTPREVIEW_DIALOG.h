/********************************************************************************
** Form generated from reading UI file 'PRINTPREVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTPREVIEW_DIALOG_H
#define UI_PRINTPREVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PrintPreview_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QComboBox *_100;
    QLabel *label_8;
    QDoubleSpinBox *_101;
    QLabel *label_9;
    QDoubleSpinBox *_102;
    QLabel *label_12;
    QComboBox *_104;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *PrintPreview_Dialog)
    {
        if (PrintPreview_Dialog->objectName().isEmpty())
            PrintPreview_Dialog->setObjectName(QString::fromUtf8("PrintPreview_Dialog"));
        PrintPreview_Dialog->resize(327, 190);
        PrintPreview_Dialog->setMinimumSize(QSize(327, 190));
        PrintPreview_Dialog->setMaximumSize(QSize(327, 190));
        QFont font;
        font.setPointSize(10);
        PrintPreview_Dialog->setFont(font);
        verticalLayout = new QVBoxLayout(PrintPreview_Dialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(6);
        label_6 = new QLabel(PrintPreview_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        _100 = new QComboBox(PrintPreview_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setFrame(true);

        gridLayout->addWidget(_100, 0, 1, 1, 1);

        label_8 = new QLabel(PrintPreview_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        gridLayout->addWidget(label_8, 1, 0, 1, 1, Qt::AlignLeft);

        _101 = new QDoubleSpinBox(PrintPreview_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setDecimals(6);
        _101->setMaximum(100000000.000000000000000);

        gridLayout->addWidget(_101, 1, 1, 1, 1);

        label_9 = new QLabel(PrintPreview_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);

        gridLayout->addWidget(label_9, 2, 0, 1, 1, Qt::AlignLeft);

        _102 = new QDoubleSpinBox(PrintPreview_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_102->sizePolicy().hasHeightForWidth());
        _102->setSizePolicy(sizePolicy);
        _102->setMinimumSize(QSize(90, 0));
        _102->setBaseSize(QSize(90, 0));
        _102->setDecimals(6);
        _102->setMaximum(100000000.000000000000000);

        gridLayout->addWidget(_102, 2, 1, 1, 1);

        label_12 = new QLabel(PrintPreview_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font);

        gridLayout->addWidget(label_12, 3, 0, 1, 1, Qt::AlignLeft);

        _104 = new QComboBox(PrintPreview_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEditable(true);

        gridLayout->addWidget(_104, 3, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(25);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, 1, 1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(PrintPreview_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(PrintPreview_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setAutoDefault(false);

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _104);
        QWidget::setTabOrder(_104, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(PrintPreview_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PrintPreview_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PrintPreview_Dialog)
    {
        PrintPreview_Dialog->setWindowTitle(QCoreApplication::translate("PrintPreview_Dialog", "Print Preview selection options", nullptr));
        label_6->setText(QCoreApplication::translate("PrintPreview_Dialog", "Paper size", nullptr));
        label_8->setText(QCoreApplication::translate("PrintPreview_Dialog", "Width", nullptr));
        label_9->setText(QCoreApplication::translate("PrintPreview_Dialog", "Height", nullptr));
        label_12->setText(QCoreApplication::translate("PrintPreview_Dialog", "Scale", nullptr));
        _1->setText(QCoreApplication::translate("PrintPreview_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("PrintPreview_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintPreview_Dialog: public Ui_PrintPreview_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTPREVIEW_DIALOG_H
