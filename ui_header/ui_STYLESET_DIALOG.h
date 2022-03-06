/********************************************************************************
** Form generated from reading UI file 'STYLESET_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STYLESET_DIALOG_H
#define UI_STYLESET_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_Styleset_Dialog
{
public:
    QPushButton *_2;
    QPushButton *_1;
    QPushButton *_112;
    QLabel *label_6;
    QComboBox *_100;
    QPushButton *_105;
    QPushButton *_111;
    QLabel *label_10;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_8;
    QPushButton *_107;
    QPushButton *_108;
    QPushButton *_109;
    QPushButton *_110;
    WeightSpinBox *_104;
    QSpinBox *_103;
    QSpinBox *_102;
    QSpinBox *_101;
    QGraphicsView *_106;

    void setupUi(QDialog *Styleset_Dialog)
    {
        if (Styleset_Dialog->objectName().isEmpty())
            Styleset_Dialog->setObjectName(QString::fromUtf8("Styleset_Dialog"));
        Styleset_Dialog->resize(452, 320);
        Styleset_Dialog->setMinimumSize(QSize(452, 320));
        Styleset_Dialog->setMaximumSize(QSize(452, 320));
        QFont font;
        font.setPointSize(10);
        Styleset_Dialog->setFont(font);
        _2 = new QPushButton(Styleset_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setEnabled(true);
        _2->setGeometry(QRect(287, 262, 75, 24));
        _2->setFont(font);
        _2->setAutoDefault(false);
        _1 = new QPushButton(Styleset_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(82, 262, 75, 24));
        _1->setFont(font);
        _1->setAutoDefault(false);
        _112 = new QPushButton(Styleset_Dialog);
        _112->setObjectName(QString::fromUtf8("_112"));
        _112->setEnabled(true);
        _112->setGeometry(QRect(185, 262, 75, 24));
        _112->setFont(font);
        _112->setAutoDefault(false);
        label_6 = new QLabel(Styleset_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(16, 22, 101, 20));
        label_6->setFont(font);
        _100 = new QComboBox(Styleset_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(120, 20, 151, 22));
        _100->setEditable(true);
        _105 = new QPushButton(Styleset_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(280, 18, 75, 24));
        _105->setFont(font);
        _105->setAutoDefault(false);
        _111 = new QPushButton(Styleset_Dialog);
        _111->setObjectName(QString::fromUtf8("_111"));
        _111->setEnabled(true);
        _111->setGeometry(QRect(364, 18, 75, 24));
        _111->setFont(font);
        _111->setAutoDefault(false);
        label_10 = new QLabel(Styleset_Dialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(75, 138, 61, 20));
        label_10->setFont(font);
        label_9 = new QLabel(Styleset_Dialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(75, 84, 61, 20));
        label_9->setFont(font);
        label_12 = new QLabel(Styleset_Dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(75, 112, 61, 20));
        label_12->setFont(font);
        label_8 = new QLabel(Styleset_Dialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(75, 60, 61, 20));
        label_8->setFont(font);
        _107 = new QPushButton(Styleset_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(300, 54, 75, 24));
        _107->setFont(font);
        _107->setAutoDefault(false);
        _108 = new QPushButton(Styleset_Dialog);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(true);
        _108->setGeometry(QRect(300, 82, 75, 24));
        _108->setFont(font);
        _108->setAutoDefault(false);
        _109 = new QPushButton(Styleset_Dialog);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(true);
        _109->setGeometry(QRect(300, 110, 75, 24));
        _109->setFont(font);
        _109->setAutoDefault(false);
        _110 = new QPushButton(Styleset_Dialog);
        _110->setObjectName(QString::fromUtf8("_110"));
        _110->setEnabled(true);
        _110->setGeometry(QRect(300, 138, 75, 24));
        _110->setFont(font);
        _110->setAutoDefault(false);
        _104 = new WeightSpinBox(Styleset_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(165, 140, 126, 22));
        _104->setMinimum(0.100000000000000);
        _104->setMaximum(3.000000000000000);
        _104->setSingleStep(0.100000000000000);
        _103 = new QSpinBox(Styleset_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(165, 112, 126, 22));
        _103->setMaximum(255);
        _102 = new QSpinBox(Styleset_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(165, 84, 126, 22));
        _102->setAccelerated(true);
        _102->setMaximum(2047);
        _101 = new QSpinBox(Styleset_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(165, 56, 126, 22));
        _101->setAccelerated(true);
        _101->setMaximum(255);
        _106 = new QGraphicsView(Styleset_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(65, 172, 311, 81));
        _106->setFrameShape(QFrame::Panel);
        _106->setMidLineWidth(0);
        _106->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _106->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _106->setInteractive(false);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _109);
        QWidget::setTabOrder(_109, _110);
        QWidget::setTabOrder(_110, _105);
        QWidget::setTabOrder(_105, _111);
        QWidget::setTabOrder(_111, _112);
        QWidget::setTabOrder(_112, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(Styleset_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(Styleset_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Styleset_Dialog)
    {
        Styleset_Dialog->setWindowTitle(QCoreApplication::translate("Styleset_Dialog", "Style Set Selection", nullptr));
        _2->setText(QCoreApplication::translate("Styleset_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("Styleset_Dialog", "OK", nullptr));
        _112->setText(QCoreApplication::translate("Styleset_Dialog", "Clone", nullptr));
        label_6->setText(QCoreApplication::translate("Styleset_Dialog", "Style set name", nullptr));
        _105->setText(QCoreApplication::translate("Styleset_Dialog", "Add", nullptr));
        _111->setText(QCoreApplication::translate("Styleset_Dialog", "Delete", nullptr));
        label_10->setText(QCoreApplication::translate("Styleset_Dialog", "Weight", nullptr));
        label_9->setText(QCoreApplication::translate("Styleset_Dialog", "Layer", nullptr));
        label_12->setText(QCoreApplication::translate("Styleset_Dialog", "Style", nullptr));
        label_8->setText(QCoreApplication::translate("Styleset_Dialog", "Colour", nullptr));
        _107->setText(QCoreApplication::translate("Styleset_Dialog", "Select", nullptr));
        _108->setText(QCoreApplication::translate("Styleset_Dialog", "Select", nullptr));
        _109->setText(QCoreApplication::translate("Styleset_Dialog", "Select", nullptr));
        _110->setText(QCoreApplication::translate("Styleset_Dialog", "Select", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Styleset_Dialog: public Ui_Styleset_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STYLESET_DIALOG_H
