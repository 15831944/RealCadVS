/********************************************************************************
** Form generated from reading UI file 'PLOTSCALE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTSCALE_DIALOG_H
#define UI_PLOTSCALE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PlotScale_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_6;
    QComboBox *_100;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *PlotScale_Dialog)
    {
        if (PlotScale_Dialog->objectName().isEmpty())
            PlotScale_Dialog->setObjectName(QString::fromUtf8("PlotScale_Dialog"));
        PlotScale_Dialog->resize(290, 100);
        PlotScale_Dialog->setMinimumSize(QSize(290, 100));
        PlotScale_Dialog->setMaximumSize(QSize(290, 100));
        QFont font;
        font.setPointSize(10);
        PlotScale_Dialog->setFont(font);
        verticalLayout = new QVBoxLayout(PlotScale_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_6 = new QLabel(PlotScale_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_6, 0, Qt::AlignLeft);

        _100 = new QComboBox(PlotScale_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setMinimumSize(QSize(134, 0));
        _100->setEditable(true);

        horizontalLayout->addWidget(_100);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        _1 = new QPushButton(PlotScale_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout_2->addWidget(_1);

        _2 = new QPushButton(PlotScale_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setAutoDefault(false);

        horizontalLayout_2->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(PlotScale_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(PlotScale_Dialog);
    } // setupUi

    void retranslateUi(QDialog *PlotScale_Dialog)
    {
        PlotScale_Dialog->setWindowTitle(QCoreApplication::translate("PlotScale_Dialog", "Change View Plot Scale", nullptr));
        label_6->setText(QCoreApplication::translate("PlotScale_Dialog", "Plot scale", nullptr));
        _1->setText(QCoreApplication::translate("PlotScale_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("PlotScale_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlotScale_Dialog: public Ui_PlotScale_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTSCALE_DIALOG_H
