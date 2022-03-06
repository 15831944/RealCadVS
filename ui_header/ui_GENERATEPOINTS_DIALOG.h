/********************************************************************************
** Form generated from reading UI file 'GENERATEPOINTS_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERATEPOINTS_DIALOG_H
#define UI_GENERATEPOINTS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/rcintegerspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_GeneratePoints_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QGroupBox *groupBox;
    QRadioButton *_102;
    QRadioButton *_103;
    QLabel *label_4;
    QLabel *label_3;
    RCDoubleSpinBox *_104;
    RCIntegerSpinBox *_105;

    void setupUi(QDialog *GeneratePoints_Dialog)
    {
        if (GeneratePoints_Dialog->objectName().isEmpty())
            GeneratePoints_Dialog->setObjectName(QString::fromUtf8("GeneratePoints_Dialog"));
        GeneratePoints_Dialog->resize(320, 191);
        QFont font;
        font.setPointSize(10);
        GeneratePoints_Dialog->setFont(font);
        _2 = new QPushButton(GeneratePoints_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(220, 160, 75, 23));
        _1 = new QPushButton(GeneratePoints_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(130, 160, 75, 23));
        groupBox = new QGroupBox(GeneratePoints_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 281, 141));
        _102 = new QRadioButton(groupBox);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(-90, 20, 281, 21));
        _102->setLayoutDirection(Qt::RightToLeft);
        _103 = new QRadioButton(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(-40, 40, 231, 21));
        _103->setLayoutDirection(Qt::RightToLeft);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 110, 191, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 80, 121, 16));
        _104 = new RCDoubleSpinBox(groupBox);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(140, 80, 116, 22));
        _104->setDecimals(6);
        _104->setMaximum(100000000.000000000000000);
        _105 = new RCIntegerSpinBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(140, 105, 116, 22));
        _105->setMaximum(100000);
        QWidget::setTabOrder(_104, _105);
        QWidget::setTabOrder(_105, _1);
        QWidget::setTabOrder(_1, _2);
        QWidget::setTabOrder(_2, _102);
        QWidget::setTabOrder(_102, _103);

        retranslateUi(GeneratePoints_Dialog);

        QMetaObject::connectSlotsByName(GeneratePoints_Dialog);
    } // setupUi

    void retranslateUi(QDialog *GeneratePoints_Dialog)
    {
        GeneratePoints_Dialog->setWindowTitle(QCoreApplication::translate("GeneratePoints_Dialog", "Generate points", nullptr));
        _2->setText(QCoreApplication::translate("GeneratePoints_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("GeneratePoints_Dialog", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("GeneratePoints_Dialog", "Spacing Options", nullptr));
        _102->setText(QCoreApplication::translate("GeneratePoints_Dialog", "By spacing        ", nullptr));
        _103->setText(QCoreApplication::translate("GeneratePoints_Dialog", "By quantity       ", nullptr));
        label_4->setText(QCoreApplication::translate("GeneratePoints_Dialog", "Number of points", nullptr));
        label_3->setText(QCoreApplication::translate("GeneratePoints_Dialog", "Spacing", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GeneratePoints_Dialog: public Ui_GeneratePoints_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERATEPOINTS_DIALOG_H
