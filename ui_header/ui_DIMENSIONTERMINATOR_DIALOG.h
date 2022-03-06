/********************************************************************************
** Form generated from reading UI file 'DIMENSIONTERMINATOR_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONTERMINATOR_DIALOG_H
#define UI_DIMENSIONTERMINATOR_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include "../RealCADQt/source/rcdoublespinbox.h"
#include "../RealCADQt/source/weightspinbox.h"

QT_BEGIN_NAMESPACE

class Ui_DimensionTerminator_Dialog
{
public:
    QLabel *label;
    QComboBox *_100;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *_107;
    QComboBox *_103;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *_104;
    QPushButton *_2;
    QPushButton *_1;
    QGraphicsView *_105;
    QPushButton *_1006;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_102;
    WeightSpinBox *_106;
    QComboBox *_203;
    QComboBox *_204;
    QComboBox *_200;

    void setupUi(QDialog *DimensionTerminator_Dialog)
    {
        if (DimensionTerminator_Dialog->objectName().isEmpty())
            DimensionTerminator_Dialog->setObjectName(QString::fromUtf8("DimensionTerminator_Dialog"));
        DimensionTerminator_Dialog->resize(377, 385);
        DimensionTerminator_Dialog->setMinimumSize(QSize(377, 385));
        DimensionTerminator_Dialog->setMaximumSize(QSize(377, 385));
        QFont font;
        font.setPointSize(10);
        DimensionTerminator_Dialog->setFont(font);
        label = new QLabel(DimensionTerminator_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(72, 10, 61, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        label->setFont(font1);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _100 = new QComboBox(DimensionTerminator_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(166, 10, 107, 22));
        _100->setFont(font1);
        label_2 = new QLabel(DimensionTerminator_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(72, 40, 61, 20));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3 = new QLabel(DimensionTerminator_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(72, 70, 61, 20));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(DimensionTerminator_Dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(72, 116, 61, 20));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _107 = new QCheckBox(DimensionTerminator_Dialog);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setGeometry(QRect(92, 90, 183, 20));
        _107->setFont(font1);
        _107->setLayoutDirection(Qt::RightToLeft);
        _103 = new QComboBox(DimensionTerminator_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(166, 142, 107, 22));
        _103->setFont(font1);
        label_5 = new QLabel(DimensionTerminator_Dialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(72, 144, 61, 20));
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(DimensionTerminator_Dialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(72, 166, 89, 20));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _104 = new QComboBox(DimensionTerminator_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(166, 164, 107, 22));
        _104->setFont(font1);
        _2 = new QPushButton(DimensionTerminator_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(280, 338, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(DimensionTerminator_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(190, 338, 75, 23));
        _1->setFont(font1);
        _105 = new QGraphicsView(DimensionTerminator_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(12, 188, 355, 145));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        _105->setFont(font2);
        _105->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _105->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _105->setSceneRect(QRectF(0, 0, 355, 145));
        _105->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        _105->setTransformationAnchor(QGraphicsView::NoAnchor);
        _105->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _1006 = new QPushButton(DimensionTerminator_Dialog);
        _1006->setObjectName(QString::fromUtf8("_1006"));
        _1006->setEnabled(true);
        _1006->setGeometry(QRect(278, 115, 24, 24));
        _1006->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1006->setIcon(icon);
        _1006->setIconSize(QSize(20, 20));
        _1006->setAutoDefault(false);
        _1006->setFlat(true);
        _101 = new RCDoubleSpinBox(DimensionTerminator_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(168, 34, 105, 24));
        _101->setFont(font1);
        _101->setMaximum(10000000.000000000000000);
        _102 = new RCDoubleSpinBox(DimensionTerminator_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(168, 60, 105, 24));
        _102->setFont(font1);
        _102->setMaximum(100000000.000000000000000);
        _106 = new WeightSpinBox(DimensionTerminator_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setGeometry(QRect(168, 114, 105, 24));
        _106->setFont(font1);
        _106->setMinimum(0.100000000000000);
        _106->setMaximum(3.000000000000000);
        _106->setSingleStep(0.100000000000000);
        _203 = new QComboBox(DimensionTerminator_Dialog);
        _203->setObjectName(QString::fromUtf8("_203"));
        _203->setGeometry(QRect(188, 138, 107, 22));
        _203->setFont(font1);
        _204 = new QComboBox(DimensionTerminator_Dialog);
        _204->setObjectName(QString::fromUtf8("_204"));
        _204->setGeometry(QRect(190, 162, 107, 22));
        _204->setFont(font1);
        _200 = new QComboBox(DimensionTerminator_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setGeometry(QRect(184, 6, 107, 22));
        _200->setFont(font1);
        _204->raise();
        _203->raise();
        _200->raise();
        label->raise();
        _100->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        _107->raise();
        _103->raise();
        label_5->raise();
        label_6->raise();
        _104->raise();
        _2->raise();
        _1->raise();
        _105->raise();
        _1006->raise();
        _101->raise();
        _102->raise();
        _106->raise();

        retranslateUi(DimensionTerminator_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionTerminator_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionTerminator_Dialog)
    {
        DimensionTerminator_Dialog->setWindowTitle(QCoreApplication::translate("DimensionTerminator_Dialog", "Dimension terminator options", nullptr));
        label->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Style", nullptr));
        label_2->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Length", nullptr));
        label_3->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Height", nullptr));
        label_4->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Weight", nullptr));
        _107->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Specify terminator weight", nullptr));
        label_5->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Position", nullptr));
        label_6->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Dimension line", nullptr));
        _2->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DimensionTerminator_Dialog", "OK", nullptr));
#if QT_CONFIG(tooltip)
        _1006->setToolTip(QCoreApplication::translate("DimensionTerminator_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1006->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1006->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DimensionTerminator_Dialog: public Ui_DimensionTerminator_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONTERMINATOR_DIALOG_H
