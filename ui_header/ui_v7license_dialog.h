/********************************************************************************
** Form generated from reading UI file 'v7license_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_V7LICENSE_DIALOG_H
#define UI_V7LICENSE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_V7License_Dialog
{
public:
    QGroupBox *groupbox1;
    QRadioButton *_104;
    QRadioButton *_105;
    QPushButton *_106;
    QGroupBox *groupBox_2;
    QRadioButton *_111;
    QRadioButton *_112;
    QLineEdit *_114;
    QLineEdit *_115;
    QPushButton *_113;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *_107;
    QLineEdit *_110;
    QPushButton *_1;
    QPushButton *_2;
    QPushButton *_120;

    void setupUi(QDialog *V7License_Dialog)
    {
        if (V7License_Dialog->objectName().isEmpty())
            V7License_Dialog->setObjectName(QString::fromUtf8("V7License_Dialog"));
        V7License_Dialog->resize(412, 422);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        V7License_Dialog->setFont(font);
        groupbox1 = new QGroupBox(V7License_Dialog);
        groupbox1->setObjectName(QString::fromUtf8("groupbox1"));
        groupbox1->setGeometry(QRect(18, 16, 377, 99));
        _104 = new QRadioButton(groupbox1);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(39, 34, 181, 20));
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setChecked(true);
        _105 = new QRadioButton(groupbox1);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(39, 62, 181, 20));
        _105->setLayoutDirection(Qt::RightToLeft);
        _106 = new QPushButton(groupbox1);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(260, 56, 113, 32));
        groupBox_2 = new QGroupBox(V7License_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(18, 124, 377, 244));
        _111 = new QRadioButton(groupBox_2);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setGeometry(QRect(12, 28, 67, 20));
        _111->setLayoutDirection(Qt::RightToLeft);
        _111->setChecked(true);
        _112 = new QRadioButton(groupBox_2);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setGeometry(QRect(16, 136, 63, 20));
        _112->setLayoutDirection(Qt::RightToLeft);
        _114 = new QLineEdit(groupBox_2);
        _114->setObjectName(QString::fromUtf8("_114"));
        _114->setGeometry(QRect(106, 60, 261, 21));
        _115 = new QLineEdit(groupBox_2);
        _115->setObjectName(QString::fromUtf8("_115"));
        _115->setGeometry(QRect(106, 92, 149, 21));
        _113 = new QPushButton(groupBox_2);
        _113->setObjectName(QString::fromUtf8("_113"));
        _113->setGeometry(QRect(260, 86, 113, 32));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(14, 64, 59, 16));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(14, 96, 87, 16));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(14, 170, 59, 16));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(14, 200, 59, 16));
        _107 = new QLineEdit(groupBox_2);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(106, 166, 149, 21));
        _110 = new QLineEdit(groupBox_2);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setGeometry(QRect(106, 198, 149, 21));
        _1 = new QPushButton(groupBox_2);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(260, 192, 113, 32));
        _2 = new QPushButton(V7License_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(274, 380, 113, 32));
        _120 = new QPushButton(V7License_Dialog);
        _120->setObjectName(QString::fromUtf8("_120"));
        _120->setGeometry(QRect(24, 381, 113, 32));
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _111);
        QWidget::setTabOrder(_111, _114);
        QWidget::setTabOrder(_114, _115);
        QWidget::setTabOrder(_115, _113);
        QWidget::setTabOrder(_113, _112);
        QWidget::setTabOrder(_112, _107);
        QWidget::setTabOrder(_107, _110);
        QWidget::setTabOrder(_110, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _120);

        retranslateUi(V7License_Dialog);

        QMetaObject::connectSlotsByName(V7License_Dialog);
    } // setupUi

    void retranslateUi(QDialog *V7License_Dialog)
    {
        V7License_Dialog->setWindowTitle(QCoreApplication::translate("V7License_Dialog", "Dialog", nullptr));
        groupbox1->setTitle(QCoreApplication::translate("V7License_Dialog", "Type of license", nullptr));
        _104->setText(QCoreApplication::translate("V7License_Dialog", "Single user license", nullptr));
        _105->setText(QCoreApplication::translate("V7License_Dialog", "Network server license", nullptr));
        _106->setText(QCoreApplication::translate("V7License_Dialog", "Network", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("V7License_Dialog", "Activation", nullptr));
        _111->setText(QCoreApplication::translate("V7License_Dialog", "Online", nullptr));
        _112->setText(QCoreApplication::translate("V7License_Dialog", "Offline", nullptr));
        _113->setText(QCoreApplication::translate("V7License_Dialog", "Get license", nullptr));
        label->setText(QCoreApplication::translate("V7License_Dialog", "Email", nullptr));
        label_2->setText(QCoreApplication::translate("V7License_Dialog", "Serial Number", nullptr));
        label_3->setText(QCoreApplication::translate("V7License_Dialog", "LOCK", nullptr));
        label_4->setText(QCoreApplication::translate("V7License_Dialog", "KEY", nullptr));
        _1->setText(QCoreApplication::translate("V7License_Dialog", "Validate", nullptr));
        _2->setText(QCoreApplication::translate("V7License_Dialog", "Cancel", nullptr));
        _120->setText(QCoreApplication::translate("V7License_Dialog", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class V7License_Dialog: public Ui_V7License_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_V7LICENSE_DIALOG_H
