/********************************************************************************
** Form generated from reading UI file 'DEFINESTYLE_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEFINESTYLE_DIALOG_H
#define UI_DEFINESTYLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DefineStyle_Dialog
{
public:
    QLabel *label_42;
    QLabel *label_44;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QPushButton *_106;
    QPushButton *_1;
    QSpinBox *_100;
    QSpinBox *_101;
    QSpinBox *_102;
    QDoubleSpinBox *_103;
    QDoubleSpinBox *_104;
    QGraphicsView *_105;
    QPushButton *_2;

    void setupUi(QDialog *DefineStyle_Dialog)
    {
        if (DefineStyle_Dialog->objectName().isEmpty())
            DefineStyle_Dialog->setObjectName(QString::fromUtf8("DefineStyle_Dialog"));
        DefineStyle_Dialog->resize(277, 280);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DefineStyle_Dialog->sizePolicy().hasHeightForWidth());
        DefineStyle_Dialog->setSizePolicy(sizePolicy);
        DefineStyle_Dialog->setMinimumSize(QSize(277, 280));
        DefineStyle_Dialog->setMaximumSize(QSize(277, 280));
        QFont font;
        font.setPointSize(8);
        DefineStyle_Dialog->setFont(font);
        label_42 = new QLabel(DefineStyle_Dialog);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(20, 10, 121, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        label_42->setFont(font1);
        label_42->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_44 = new QLabel(DefineStyle_Dialog);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(20, 40, 141, 20));
        label_44->setFont(font1);
        label_44->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_46 = new QLabel(DefineStyle_Dialog);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(20, 70, 141, 20));
        label_46->setFont(font1);
        label_46->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_47 = new QLabel(DefineStyle_Dialog);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(46, 104, 81, 20));
        label_47->setFont(font1);
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_48 = new QLabel(DefineStyle_Dialog);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(46, 134, 51, 20));
        label_48->setFont(font1);
        label_48->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _106 = new QPushButton(DefineStyle_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(100, 228, 75, 25));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        _106->setFont(font2);
        _106->setAutoDefault(false);
        _1 = new QPushButton(DefineStyle_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(18, 228, 75, 25));
        _1->setFont(font2);
        _1->setAutoDefault(false);
        _100 = new QSpinBox(DefineStyle_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(170, 8, 87, 22));
        _100->setMaximum(10000);
        _101 = new QSpinBox(DefineStyle_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(170, 38, 87, 22));
        _101->setMaximum(10000);
        _102 = new QSpinBox(DefineStyle_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(170, 68, 87, 22));
        _102->setMinimum(1);
        _102->setMaximum(10000);
        _103 = new QDoubleSpinBox(DefineStyle_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(134, 104, 87, 22));
        _103->setMaximum(100000000.000000000000000);
        _103->setSingleStep(0.100000000000000);
        _104 = new QDoubleSpinBox(DefineStyle_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(134, 134, 87, 22));
        _104->setMaximum(100000000.000000000000000);
        _104->setSingleStep(0.100000000000000);
        _105 = new QGraphicsView(DefineStyle_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(14, 162, 249, 61));
        _2 = new QPushButton(DefineStyle_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(182, 228, 75, 25));
        _2->setFont(font2);
        _2->setAutoDefault(false);
        QWidget::setTabOrder(_100, _101);
        QWidget::setTabOrder(_101, _102);
        QWidget::setTabOrder(_102, _103);
        QWidget::setTabOrder(_103, _104);
        QWidget::setTabOrder(_104, _1);
        QWidget::setTabOrder(_1, _106);
        QWidget::setTabOrder(_106, _105);

        retranslateUi(DefineStyle_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DefineStyle_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DefineStyle_Dialog)
    {
        DefineStyle_Dialog->setWindowTitle(QCoreApplication::translate("DefineStyle_Dialog", "Define dashed line style", nullptr));
        label_42->setText(QCoreApplication::translate("DefineStyle_Dialog", "Style index", nullptr));
        label_44->setText(QCoreApplication::translate("DefineStyle_Dialog", "Number of segments", nullptr));
        label_46->setText(QCoreApplication::translate("DefineStyle_Dialog", "Change segment", nullptr));
        label_47->setText(QCoreApplication::translate("DefineStyle_Dialog", "Dash length", nullptr));
        label_48->setText(QCoreApplication::translate("DefineStyle_Dialog", "Space", nullptr));
        _106->setText(QCoreApplication::translate("DefineStyle_Dialog", "Apply", nullptr));
        _1->setText(QCoreApplication::translate("DefineStyle_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("DefineStyle_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DefineStyle_Dialog: public Ui_DefineStyle_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEFINESTYLE_DIALOG_H
