/********************************************************************************
** Form generated from reading UI file 'SUNPOSITION_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUNPOSITION_DIALOG_H
#define UI_SUNPOSITION_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_SunPosition_Dialog
{
public:
    QLabel *label_9;
    QLineEdit *_104;
    QPushButton *_2;
    QLineEdit *_105;
    QComboBox *_102;
    QLineEdit *_103;
    QPushButton *_1;
    QPushButton *_106;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_7;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_8;
    QLabel *label_10;
    QTimeEdit *_101;
    QDateEdit *_100;
    QPushButton *_107;
    QCheckBox *_108;

    void setupUi(QDialog *SunPosition_Dialog)
    {
        if (SunPosition_Dialog->objectName().isEmpty())
            SunPosition_Dialog->setObjectName(QString::fromUtf8("SunPosition_Dialog"));
        SunPosition_Dialog->resize(384, 255);
        SunPosition_Dialog->setMinimumSize(QSize(384, 255));
        SunPosition_Dialog->setMaximumSize(QSize(384, 255));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        SunPosition_Dialog->setFont(font);
        label_9 = new QLabel(SunPosition_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(24, 156, 121, 16));
        _104 = new QLineEdit(SunPosition_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(160, 126, 100, 23));
        _2 = new QPushButton(SunPosition_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(250, 212, 100, 25));
        _2->setAutoDefault(false);
        _105 = new QLineEdit(SunPosition_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(160, 154, 100, 23));
        _102 = new QComboBox(SunPosition_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(158, 68, 203, 22));
        _102->setEditable(true);
        _103 = new QLineEdit(SunPosition_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(160, 98, 100, 23));
        _1 = new QPushButton(SunPosition_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setEnabled(true);
        _1->setGeometry(QRect(30, 212, 100, 25));
        _106 = new QPushButton(SunPosition_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(140, 212, 100, 25));
        _106->setAutoDefault(false);
        label_6 = new QLabel(SunPosition_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(24, 100, 121, 16));
        label_2 = new QLabel(SunPosition_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(25, 72, 119, 16));
        label_7 = new QLabel(SunPosition_Dialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(24, 128, 121, 16));
        label_13 = new QLabel(SunPosition_Dialog);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(268, 101, 61, 16));
        label_14 = new QLabel(SunPosition_Dialog);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(268, 127, 61, 16));
        label_8 = new QLabel(SunPosition_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(24, 42, 121, 16));
        label_10 = new QLabel(SunPosition_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(24, 14, 121, 16));
        _101 = new QTimeEdit(SunPosition_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(160, 39, 140, 22));
        _101->setCalendarPopup(false);
        _100 = new QDateEdit(SunPosition_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(158, 10, 140, 22));
        _100->setCalendarPopup(true);
        _107 = new QPushButton(SunPosition_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(false);
        _107->setGeometry(QRect(318, 10, 31, 21));
        _107->setAutoDefault(false);
        _108 = new QCheckBox(SunPosition_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(72, 188, 189, 20));
        _108->setLayoutDirection(Qt::RightToLeft);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _108);
        QWidget::setTabOrder(_108, _106);
        QWidget::setTabOrder(_106, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _107);

        retranslateUi(SunPosition_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SunPosition_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SunPosition_Dialog)
    {
        SunPosition_Dialog->setWindowTitle(QCoreApplication::translate("SunPosition_Dialog", "Sun Position", nullptr));
        label_9->setText(QCoreApplication::translate("SunPosition_Dialog", "Time Zone", nullptr));
        _2->setText(QCoreApplication::translate("SunPosition_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("SunPosition_Dialog", "OK", nullptr));
        _106->setText(QCoreApplication::translate("SunPosition_Dialog", "North Direction", nullptr));
        label_6->setText(QCoreApplication::translate("SunPosition_Dialog", "Longitude", nullptr));
        label_2->setText(QCoreApplication::translate("SunPosition_Dialog", "City", nullptr));
        label_7->setText(QCoreApplication::translate("SunPosition_Dialog", "Latitude", nullptr));
        label_13->setText(QCoreApplication::translate("SunPosition_Dialog", "deg. min", nullptr));
        label_14->setText(QCoreApplication::translate("SunPosition_Dialog", "deg. min", nullptr));
        label_8->setText(QCoreApplication::translate("SunPosition_Dialog", "Time", nullptr));
        label_10->setText(QCoreApplication::translate("SunPosition_Dialog", "Date", nullptr));
        _101->setDisplayFormat(QCoreApplication::translate("SunPosition_Dialog", "H:mm:ss", nullptr));
        _107->setText(QCoreApplication::translate("SunPosition_Dialog", "dsplay control do not remove", nullptr));
        _108->setText(QCoreApplication::translate("SunPosition_Dialog", "keep existing sun lights", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SunPosition_Dialog: public Ui_SunPosition_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUNPOSITION_DIALOG_H
