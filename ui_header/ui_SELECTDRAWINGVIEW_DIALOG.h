/********************************************************************************
** Form generated from reading UI file 'SELECTDRAWINGVIEW_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTDRAWINGVIEW_DIALOG_H
#define UI_SELECTDRAWINGVIEW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rccombolist.h"

QT_BEGIN_NAMESPACE

class Ui_SelectDrawingView_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_9;
    QCheckBox *_102;
    QCheckBox *_103;
    RCComboList *_100;
    QDoubleSpinBox *_106;
    QDoubleSpinBox *_104;
    QCheckBox *_105;
    RCComboList *_101;
    QLabel *label_2;

    void setupUi(QDialog *SelectDrawingView_Dialog)
    {
        if (SelectDrawingView_Dialog->objectName().isEmpty())
            SelectDrawingView_Dialog->setObjectName(QString::fromUtf8("SelectDrawingView_Dialog"));
        SelectDrawingView_Dialog->resize(530, 377);
        SelectDrawingView_Dialog->setMinimumSize(QSize(507, 377));
        SelectDrawingView_Dialog->setMaximumSize(QSize(600, 600));
        QFont font;
        font.setPointSize(10);
        SelectDrawingView_Dialog->setFont(font);
        _2 = new QPushButton(SelectDrawingView_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(326, 344, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(SelectDrawingView_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(236, 344, 75, 23));
        _1->setFont(font1);
        label = new QLabel(SelectDrawingView_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(28, 10, 227, 20));
        label->setFont(font);
        label_8 = new QLabel(SelectDrawingView_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(162, 214, 69, 20));
        label_8->setFont(font);
        label_9 = new QLabel(SelectDrawingView_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(162, 242, 67, 20));
        label_9->setFont(font);
        _102 = new QCheckBox(SelectDrawingView_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(142, 290, 181, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setAutoFillBackground(false);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        _103 = new QCheckBox(SelectDrawingView_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(134, 310, 189, 20));
        _103->setFont(font);
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setAutoFillBackground(false);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _100 = new RCComboList(SelectDrawingView_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(30, 30, 215, 169));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_100->sizePolicy().hasHeightForWidth());
        _100->setSizePolicy(sizePolicy);
        _100->setMinimumSize(QSize(10, 10));
        _100->setMaximumSize(QSize(250, 169));
        _100->setProperty("autoScrollMargin", QVariant(16));
        _100->setProperty("uniformItemSizes", QVariant(true));
        _106 = new QDoubleSpinBox(SelectDrawingView_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(236, 212, 99, 24));
        _106->setMaximum(1000000000.000000000000000);
        _104 = new QDoubleSpinBox(SelectDrawingView_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(236, 240, 99, 24));
        _104->setWrapping(true);
        _104->setMaximum(360.000000000000000);
        _105 = new QCheckBox(SelectDrawingView_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(134, 270, 189, 20));
        _105->setFont(font);
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setAutoFillBackground(false);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _101 = new RCComboList(SelectDrawingView_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(262, 30, 215, 169));
        sizePolicy.setHeightForWidth(_101->sizePolicy().hasHeightForWidth());
        _101->setSizePolicy(sizePolicy);
        _101->setMinimumSize(QSize(10, 10));
        _101->setMaximumSize(QSize(250, 169));
        _101->setProperty("autoScrollMargin", QVariant(16));
        _101->setProperty("uniformItemSizes", QVariant(true));
        label_2 = new QLabel(SelectDrawingView_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(274, 10, 227, 20));
        label_2->setFont(font);
        QWidget::setTabOrder(_100, _106);
        QWidget::setTabOrder(_106, _104);
        QWidget::setTabOrder(_104, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(SelectDrawingView_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SelectDrawingView_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SelectDrawingView_Dialog)
    {
        SelectDrawingView_Dialog->setWindowTitle(QCoreApplication::translate("SelectDrawingView_Dialog", "Select 3D view for layout drawing", nullptr));
        _2->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "OK", nullptr));
        label->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Select the 3D view to place on the drawing", nullptr));
        label_8->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "View scale", nullptr));
        label_9->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Angle", nullptr));
        _102->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Hide view when plotting", nullptr));
        _103->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Dash hidden lines when hiding", nullptr));
        _105->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Shade view when plotting", nullptr));
        label_2->setText(QCoreApplication::translate("SelectDrawingView_Dialog", "Select the Zoom Zone to use", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectDrawingView_Dialog: public Ui_SelectDrawingView_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDRAWINGVIEW_DIALOG_H
