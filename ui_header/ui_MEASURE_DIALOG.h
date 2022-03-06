/********************************************************************************
** Form generated from reading UI file 'MEASURE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASURE_DIALOG_H
#define UI_MEASURE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Measure_Dialog
{
public:
    QLabel *label_4;
    QGroupBox *groupBox;
    QLineEdit *_100;
    QLabel *label_5;
    QLineEdit *_103;
    QLabel *label_6;
    QLineEdit *_101;
    QLineEdit *_104;
    QLineEdit *_105;
    QLineEdit *_102;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *_106;
    QLabel *label_12;
    QLabel *label_13;
    RCDoubleSpinBox *_108;
    QCheckBox *_107;
    QGroupBox *groupBox_2;
    QLineEdit *_109;
    QLabel *label_11;
    QLineEdit *_111;
    QLabel *label_10;
    QLineEdit *_110;
    QLabel *label_9;
    QGroupBox *groupBox_3;
    QRadioButton *_120;
    QRadioButton *_121;

    void setupUi(QDialog *Measure_Dialog)
    {
        if (Measure_Dialog->objectName().isEmpty())
            Measure_Dialog->setObjectName(QString::fromUtf8("Measure_Dialog"));
        Measure_Dialog->resize(416, 407);
        Measure_Dialog->setMinimumSize(QSize(416, 407));
        Measure_Dialog->setSizeIncrement(QSize(416, 407));
        label_4 = new QLabel(Measure_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(212, 111, 53, 20));
        QFont font;
        font.setPointSize(10);
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox = new QGroupBox(Measure_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(16, 255, 385, 143));
        _100 = new QLineEdit(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(96, 30, 131, 20));
        _100->setReadOnly(true);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(15, 30, 71, 20));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _103 = new QLineEdit(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(236, 30, 131, 20));
        _103->setReadOnly(true);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(15, 57, 71, 20));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QLineEdit(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(96, 57, 131, 20));
        _101->setReadOnly(true);
        _104 = new QLineEdit(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(236, 57, 131, 20));
        _104->setReadOnly(true);
        _105 = new QLineEdit(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(236, 84, 131, 20));
        _105->setReadOnly(true);
        _102 = new QLineEdit(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(96, 84, 131, 20));
        _102->setReadOnly(true);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(15, 84, 71, 20));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(15, 110, 81, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QLineEdit(groupBox);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(96, 110, 271, 20));
        _106->setReadOnly(true);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(138, 7, 49, 20));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignCenter);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(274, 7, 49, 20));
        label_13->setFont(font);
        label_13->setAlignment(Qt::AlignCenter);
        _108 = new RCDoubleSpinBox(Measure_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(266, 108, 100, 24));
        _108->setAccelerated(true);
        _108->setMinimum(-100000000.000000000000000);
        _108->setMaximum(100000000.000000000000000);
        _107 = new QCheckBox(Measure_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(60, 110, 131, 20));
        _107->setLayoutDirection(Qt::LeftToRight);
        groupBox_2 = new QGroupBox(Measure_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(16, 137, 385, 102));
        _109 = new QLineEdit(groupBox_2);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setGeometry(QRect(232, 14, 133, 20));
        _109->setReadOnly(true);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(94, 41, 137, 20));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _111 = new QLineEdit(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(232, 68, 133, 20));
        _111->setReadOnly(true);
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(94, 68, 137, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _110 = new QLineEdit(groupBox_2);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(232, 41, 133, 20));
        _110->setReadOnly(true);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(94, 14, 137, 20));
        label_9->setFont(font);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(Measure_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 8, 395, 94));
        _120 = new QRadioButton(groupBox_3);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(50, 30, 307, 20));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_120->sizePolicy().hasHeightForWidth());
        _120->setSizePolicy(sizePolicy);
        _120->setLayoutDirection(Qt::RightToLeft);
        _121 = new QRadioButton(groupBox_3);
        _121->setObjectName(QString::fromUtf8("_121"));
        _121->setGeometry(QRect(50, 56, 307, 20));
        _121->setLayoutDirection(Qt::RightToLeft);
        QWidget::setTabOrder(_120, _121);
        QWidget::setTabOrder(_121, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _111);
        QWidget::setTabOrder(_111, _100);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _106);
        QWidget::setTabOrder(_106, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);

        retranslateUi(Measure_Dialog);

        QMetaObject::connectSlotsByName(Measure_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Measure_Dialog)
    {
        Measure_Dialog->setWindowTitle(QCoreApplication::translate("Measure_Dialog", "Measure options", nullptr));
        label_4->setText(QCoreApplication::translate("Measure_Dialog", "Scale", nullptr));
        groupBox->setTitle(QString());
        label_5->setText(QCoreApplication::translate("Measure_Dialog", "Number", nullptr));
        label_6->setText(QCoreApplication::translate("Measure_Dialog", "Last", nullptr));
        label_7->setText(QCoreApplication::translate("Measure_Dialog", "Total", nullptr));
        label_8->setText(QCoreApplication::translate("Measure_Dialog", "Last direction", nullptr));
        label_12->setText(QCoreApplication::translate("Measure_Dialog", "Length", nullptr));
        label_13->setText(QCoreApplication::translate("Measure_Dialog", "Area", nullptr));
        _107->setText(QCoreApplication::translate("Measure_Dialog", "Enable scale", nullptr));
        groupBox_2->setTitle(QString());
        label_11->setText(QCoreApplication::translate("Measure_Dialog", "Angle two", nullptr));
        label_10->setText(QCoreApplication::translate("Measure_Dialog", "Distance between lines", nullptr));
        label_9->setText(QCoreApplication::translate("Measure_Dialog", "Angle one", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Measure_Dialog", "Measure Options", nullptr));
        _120->setText(QCoreApplication::translate("Measure_Dialog", "Area and Length                                            ", nullptr));
        _121->setText(QCoreApplication::translate("Measure_Dialog", "Angle and Offset Distance                             ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Measure_Dialog: public Ui_Measure_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASURE_DIALOG_H
