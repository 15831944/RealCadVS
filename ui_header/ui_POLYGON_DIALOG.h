/********************************************************************************
** Form generated from reading UI file 'POLYGON_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POLYGON_DIALOG_H
#define UI_POLYGON_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Polygon_Dialog
{
public:
    QLabel *label_9;
    QLabel *label_8;
    QGroupBox *groupBox;
    QRadioButton *_102;
    QRadioButton *_103;
    QRadioButton *_104;
    QLabel *label_10;
    QLabel *label_11;
    RCDoubleSpinBox *_105;
    RCDoubleSpinBox *_106;
    QCheckBox *_107;
    QCheckBox *_108;
    QPushButton *_2;
    QPushButton *_1;
    RCIntegerSpinBox *_100;
    RCDoubleSpinBox *_101;

    void setupUi(QDialog *Polygon_Dialog)
    {
        if (Polygon_Dialog->objectName().isEmpty())
            Polygon_Dialog->setObjectName(QString::fromUtf8("Polygon_Dialog"));
        Polygon_Dialog->resize(250, 320);
        Polygon_Dialog->setMinimumSize(QSize(250, 320));
        Polygon_Dialog->setMaximumSize(QSize(250, 320));
        QFont font;
        font.setPointSize(10);
        Polygon_Dialog->setFont(font);
        label_9 = new QLabel(Polygon_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 40, 101, 20));
        label_9->setFont(font);
        label_8 = new QLabel(Polygon_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 16, 101, 20));
        label_8->setFont(font);
        groupBox = new QGroupBox(Polygon_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 70, 231, 141));
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(50, 20, 151, 17));
        _102->setLayoutDirection(Qt::LeftToRight);
        _103 = new QRadioButton(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(50, 42, 141, 17));
        _103->setLayoutDirection(Qt::LeftToRight);
        _104 = new QRadioButton(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(50, 64, 151, 17));
        _104->setLayoutDirection(Qt::LeftToRight);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 110, 101, 20));
        label_10->setFont(font);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 86, 101, 20));
        label_11->setFont(font);
        _105 = new RCDoubleSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(130, 85, 86, 22));
        _105->setDecimals(6);
        _105->setMaximum(100000000.000000000000000);
        _106 = new RCDoubleSpinBox(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(130, 110, 86, 22));
        _106->setDecimals(6);
        _106->setMaximum(100000000.000000000000000);
        _107 = new QCheckBox(Polygon_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(23, 215, 181, 21));
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);
        _108 = new QCheckBox(Polygon_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(true);
        _108->setGeometry(QRect(22, 240, 181, 21));
        _108->setLayoutDirection(Qt::RightToLeft);
        _108->setCheckable(true);
        _108->setChecked(false);
        _108->setTristate(false);
        _2 = new QPushButton(Polygon_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(160, 270, 75, 23));
        _1 = new QPushButton(Polygon_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(70, 270, 75, 23));
        _100 = new RCIntegerSpinBox(Polygon_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(140, 15, 86, 22));
        _100->setMaximum(10000);
        _101 = new RCDoubleSpinBox(Polygon_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(140, 40, 86, 22));
        _101->setDecimals(6);
        _101->setMaximum(100000000.000000000000000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Polygon_Dialog);

        QMetaObject::connectSlotsByName(Polygon_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Polygon_Dialog)
    {
        Polygon_Dialog->setWindowTitle(QCoreApplication::translate("Polygon_Dialog", "Insert regular polygon options", nullptr));
        label_9->setText(QCoreApplication::translate("Polygon_Dialog", "Thickness", nullptr));
        label_8->setText(QCoreApplication::translate("Polygon_Dialog", "Number of sides", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Polygon_Dialog", "Size", nullptr));
        _102->setText(QCoreApplication::translate("Polygon_Dialog", "Not fixed", nullptr));
        _103->setText(QCoreApplication::translate("Polygon_Dialog", "Fix radius", nullptr));
        _104->setText(QCoreApplication::translate("Polygon_Dialog", "Fix chord length", nullptr));
        label_10->setText(QCoreApplication::translate("Polygon_Dialog", "Chord length", nullptr));
        label_11->setText(QCoreApplication::translate("Polygon_Dialog", "Radius", nullptr));
        _107->setText(QCoreApplication::translate("Polygon_Dialog", "Local vertex point", nullptr));
        _108->setText(QCoreApplication::translate("Polygon_Dialog", "Hatch", nullptr));
        _2->setText(QCoreApplication::translate("Polygon_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Polygon_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Polygon_Dialog: public Ui_Polygon_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POLYGON_DIALOG_H
