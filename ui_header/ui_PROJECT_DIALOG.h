/********************************************************************************
** Form generated from reading UI file 'PROJECT_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECT_DIALOG_H
#define UI_PROJECT_DIALOG_H

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

class Ui_Project_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_100;
    QRadioButton *_101;
    QRadioButton *_102;
    QRadioButton *_103;
    QRadioButton *_104;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_12;
    QLabel *label_11;
    QCheckBox *_109;
    RCIntegerSpinBox *_108;
    RCDoubleSpinBox *_107;
    RCDoubleSpinBox *_106;
    RCDoubleSpinBox *_105;

    void setupUi(QDialog *Project_Dialog)
    {
        if (Project_Dialog->objectName().isEmpty())
            Project_Dialog->setObjectName(QString::fromUtf8("Project_Dialog"));
        Project_Dialog->resize(375, 360);
        Project_Dialog->setMinimumSize(QSize(375, 360));
        Project_Dialog->setMaximumSize(QSize(375, 360));
        QFont font;
        font.setPointSize(10);
        Project_Dialog->setFont(font);
        _2 = new QPushButton(Project_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(286, 310, 75, 23));
        _2->setAutoDefault(false);
        _1 = new QPushButton(Project_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(196, 310, 75, 23));
        groupBox = new QGroupBox(Project_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(13, 9, 351, 141));
        groupBox->setFont(font);
        _100 = new QRadioButton(groupBox);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(20, 23, 301, 20));
        _100->setFont(font);
        _100->setLayoutDirection(Qt::RightToLeft);
        _101 = new QRadioButton(groupBox);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(20, 44, 301, 20));
        _101->setFont(font);
        _101->setLayoutDirection(Qt::RightToLeft);
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(20, 65, 301, 20));
        _102->setFont(font);
        _102->setLayoutDirection(Qt::RightToLeft);
        _103 = new QRadioButton(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(20, 86, 301, 20));
        _103->setFont(font);
        _103->setLayoutDirection(Qt::RightToLeft);
        _104 = new QRadioButton(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(20, 108, 301, 20));
        _104->setFont(font);
        _104->setLayoutDirection(Qt::RightToLeft);
        label_8 = new QLabel(Project_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(89, 246, 101, 20));
        label_8->setFont(font);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_10 = new QLabel(Project_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(89, 218, 91, 20));
        label_10->setFont(font);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_12 = new QLabel(Project_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(89, 191, 91, 20));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_11 = new QLabel(Project_Dialog);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(89, 163, 91, 20));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _109 = new QCheckBox(Project_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(true);
        _109->setGeometry(QRect(76, 280, 171, 21));
        _109->setLayoutDirection(Qt::RightToLeft);
        _109->setCheckable(true);
        _109->setChecked(false);
        _109->setTristate(false);
        _108 = new RCIntegerSpinBox(Project_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setGeometry(QRect(200, 244, 99, 24));
        _108->setMinimum(1);
        _108->setMaximum(1000000);
        _107 = new RCDoubleSpinBox(Project_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(200, 216, 99, 24));
        _107->setMinimum(-1000000000.000000000000000);
        _107->setMaximum(1000000000.000000000000000);
        _106 = new RCDoubleSpinBox(Project_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(200, 188, 99, 24));
        _106->setMinimum(-1000000000.000000000000000);
        _106->setMaximum(1000000000.000000000000000);
        _105 = new RCDoubleSpinBox(Project_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(200, 160, 99, 24));
        _105->setMinimum(-1000000000.000000000000000);
        _105->setMaximum(1000000000.000000000000000);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _106);
        QWidget::setTabOrder(_106, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Project_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Project_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Project_Dialog)
    {
        Project_Dialog->setWindowTitle(QCoreApplication::translate("Project_Dialog", "Project Options", nullptr));
        _2->setText(QCoreApplication::translate("Project_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Project_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Project_Dialog", "Define projection by", nullptr));
        _100->setText(QCoreApplication::translate("Project_Dialog", "Coordinate entry", nullptr));
        _101->setText(QCoreApplication::translate("Project_Dialog", "Two points", nullptr));
        _102->setText(QCoreApplication::translate("Project_Dialog", "Plane defined by two points or a line", nullptr));
        _103->setText(QCoreApplication::translate("Project_Dialog", "Plane defined by three points, a circle or a plane", nullptr));
        _104->setText(QCoreApplication::translate("Project_Dialog", "Entity chain", nullptr));
        label_8->setText(QCoreApplication::translate("Project_Dialog", "Number of copies", nullptr));
        label_10->setText(QCoreApplication::translate("Project_Dialog", "Delta Z", nullptr));
        label_12->setText(QCoreApplication::translate("Project_Dialog", "Delta Y", nullptr));
        label_11->setText(QCoreApplication::translate("Project_Dialog", "Delta X", nullptr));
        _109->setText(QCoreApplication::translate("Project_Dialog", "Insert surfaces", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Project_Dialog: public Ui_Project_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECT_DIALOG_H
