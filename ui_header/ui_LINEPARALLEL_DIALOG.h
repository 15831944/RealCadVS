/********************************************************************************
** Form generated from reading UI file 'LINEPARALLEL_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEPARALLEL_DIALOG_H
#define UI_LINEPARALLEL_DIALOG_H

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

class Ui_LineParallel_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QLabel *label_9;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QRadioButton *_102;
    QLabel *label_11;
    QCheckBox *_105;
    QCheckBox *_106;
    QCheckBox *_107;
    RCDoubleSpinBox *_104;
    RCIntegerSpinBox *_103;

    void setupUi(QDialog *LineParallel_Dialog)
    {
        if (LineParallel_Dialog->objectName().isEmpty())
            LineParallel_Dialog->setObjectName(QString::fromUtf8("LineParallel_Dialog"));
        LineParallel_Dialog->resize(282, 280);
        LineParallel_Dialog->setMinimumSize(QSize(282, 280));
        LineParallel_Dialog->setMaximumSize(QSize(282, 280));
        QFont font;
        font.setPointSize(10);
        LineParallel_Dialog->setFont(font);
        _2 = new QPushButton(LineParallel_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(180, 230, 75, 23));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        _2->setFont(font1);
        _1 = new QPushButton(LineParallel_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(90, 230, 75, 23));
        _1->setFont(font1);
        label_9 = new QLabel(LineParallel_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(24, 128, 131, 21));
        label_9->setFont(font1);
        groupBox = new QGroupBox(LineParallel_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 246, 101));
        groupBox->setFont(font1);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(60, 30, 101, 17));
        _100->setFont(font1);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(60, 50, 101, 17));
        _101->setFont(font1);
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(60, 70, 101, 17));
        _102->setFont(font1);
        _102->setLayoutDirection(Qt::RightToLeft);
        label_11 = new QLabel(LineParallel_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(22, 151, 169, 21));
        label_11->setFont(font1);
        _105 = new QCheckBox(LineParallel_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(10, 180, 101, 21));
        _105->setFont(font1);
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _106 = new QCheckBox(LineParallel_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(10, 200, 101, 21));
        _106->setFont(font1);
        _106->setLayoutDirection(Qt::RightToLeft);
        _106->setCheckable(true);
        _106->setChecked(false);
        _106->setTristate(false);
        _107 = new QCheckBox(LineParallel_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(140, 200, 101, 21));
        _107->setFont(font1);
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);
        _104 = new RCDoubleSpinBox(LineParallel_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(195, 150, 71, 22));
        _104->setFont(font1);
        _104->setDecimals(6);
        _104->setMinimum(-10000000.000000000000000);
        _104->setMaximum(10000000.000000000000000);
        _103 = new RCIntegerSpinBox(LineParallel_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(195, 125, 71, 22));
        _103->setFont(font1);
        _103->setMaximum(10000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(LineParallel_Dialog);

        QMetaObject::connectSlotsByName(LineParallel_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LineParallel_Dialog)
    {
        LineParallel_Dialog->setWindowTitle(QCoreApplication::translate("LineParallel_Dialog", "Insert multiple parallel lines", nullptr));
        _2->setText(QCoreApplication::translate("LineParallel_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("LineParallel_Dialog", "OK", nullptr));
        label_9->setText(QCoreApplication::translate("LineParallel_Dialog", "Number of lines", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LineParallel_Dialog", "Digitized line is the ", nullptr));
        _100->setText(QCoreApplication::translate("LineParallel_Dialog", "Left line", nullptr));
        _101->setText(QCoreApplication::translate("LineParallel_Dialog", "Centre line", nullptr));
        _102->setText(QCoreApplication::translate("LineParallel_Dialog", "Right line", nullptr));
        label_11->setText(QCoreApplication::translate("LineParallel_Dialog", "Offset distance between lines", nullptr));
        _105->setText(QCoreApplication::translate("LineParallel_Dialog", "Closed loop", nullptr));
        _106->setText(QCoreApplication::translate("LineParallel_Dialog", "Close start", nullptr));
        _107->setText(QCoreApplication::translate("LineParallel_Dialog", "Close end", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LineParallel_Dialog: public Ui_LineParallel_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEPARALLEL_DIALOG_H
