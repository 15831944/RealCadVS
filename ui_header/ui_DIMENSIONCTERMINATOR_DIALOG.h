/********************************************************************************
** Form generated from reading UI file 'DIMENSIONCTERMINATOR_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIMENSIONCTERMINATOR_DIALOG_H
#define UI_DIMENSIONCTERMINATOR_DIALOG_H

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

class Ui_DimensionCTerminator_Dialog
{
public:
    QComboBox *_100;
    QLabel *label_45;
    QCheckBox *_105;
    QLabel *label_53;
    QLabel *label_54;
    QLabel *label_55;
    QPushButton *_2;
    QPushButton *_1;
    RCDoubleSpinBox *_101;
    RCDoubleSpinBox *_102;
    WeightSpinBox *_104;
    QGraphicsView *_103;
    QPushButton *_1007;
    QComboBox *_200;

    void setupUi(QDialog *DimensionCTerminator_Dialog)
    {
        if (DimensionCTerminator_Dialog->objectName().isEmpty())
            DimensionCTerminator_Dialog->setObjectName(QString::fromUtf8("DimensionCTerminator_Dialog"));
        DimensionCTerminator_Dialog->resize(358, 355);
        DimensionCTerminator_Dialog->setMinimumSize(QSize(358, 355));
        DimensionCTerminator_Dialog->setMaximumSize(QSize(358, 355));
        QFont font;
        font.setPointSize(10);
        DimensionCTerminator_Dialog->setFont(font);
        _100 = new QComboBox(DimensionCTerminator_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(152, 8, 106, 22));
        label_45 = new QLabel(DimensionCTerminator_Dialog);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(78, 10, 71, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        label_45->setFont(font1);
        label_45->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _105 = new QCheckBox(DimensionCTerminator_Dialog);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setGeometry(QRect(70, 88, 189, 21));
        _105->setFont(font1);
        _105->setLayoutDirection(Qt::RightToLeft);
        label_53 = new QLabel(DimensionCTerminator_Dialog);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(78, 34, 71, 20));
        label_53->setFont(font1);
        label_53->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_54 = new QLabel(DimensionCTerminator_Dialog);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(78, 60, 71, 20));
        label_54->setFont(font1);
        label_54->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_55 = new QLabel(DimensionCTerminator_Dialog);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(78, 116, 71, 20));
        label_55->setFont(font1);
        label_55->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DimensionCTerminator_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(256, 304, 75, 23));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _1 = new QPushButton(DimensionCTerminator_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(166, 304, 75, 23));
        _1->setFont(font1);
        _101 = new RCDoubleSpinBox(DimensionCTerminator_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(152, 32, 105, 24));
        _101->setMaximum(100000000.000000000000000);
        _102 = new RCDoubleSpinBox(DimensionCTerminator_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setGeometry(QRect(152, 58, 105, 24));
        _102->setMaximum(100000000.000000000000000);
        _104 = new WeightSpinBox(DimensionCTerminator_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setGeometry(QRect(152, 112, 105, 24));
        _104->setMinimum(0.100000000000000);
        _104->setMaximum(3.000000000000000);
        _104->setSingleStep(0.100000000000000);
        _103 = new QGraphicsView(DimensionCTerminator_Dialog);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setGeometry(QRect(12, 141, 335, 158));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        _103->setFont(font2);
        _103->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _103->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _103->setSceneRect(QRectF(0, 0, 335, 158));
        _103->setTransformationAnchor(QGraphicsView::NoAnchor);
        _103->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        _1007 = new QPushButton(DimensionCTerminator_Dialog);
        _1007->setObjectName(QString::fromUtf8("_1007"));
        _1007->setEnabled(true);
        _1007->setGeometry(QRect(266, 112, 24, 24));
        _1007->setFont(font1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/PROPERTIES-weights.svg"), QSize(), QIcon::Normal, QIcon::Off);
        _1007->setIcon(icon);
        _1007->setIconSize(QSize(20, 20));
        _1007->setAutoDefault(false);
        _1007->setFlat(true);
        _200 = new QComboBox(DimensionCTerminator_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setGeometry(QRect(164, 4, 106, 22));
        _200->raise();
        _100->raise();
        label_45->raise();
        _105->raise();
        label_53->raise();
        label_54->raise();
        label_55->raise();
        _2->raise();
        _1->raise();
        _101->raise();
        _102->raise();
        _104->raise();
        _103->raise();
        _1007->raise();

        retranslateUi(DimensionCTerminator_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(DimensionCTerminator_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DimensionCTerminator_Dialog)
    {
        DimensionCTerminator_Dialog->setWindowTitle(QCoreApplication::translate("DimensionCTerminator_Dialog", "Dimension terminator options", nullptr));
        label_45->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Style", nullptr));
        _105->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Specify terminator weight", nullptr));
        label_53->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Length", nullptr));
        label_54->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Height", nullptr));
        label_55->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Weight", nullptr));
        _2->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("DimensionCTerminator_Dialog", "OK", nullptr));
#if QT_CONFIG(tooltip)
        _1007->setToolTip(QCoreApplication::translate("DimensionCTerminator_Dialog", "Select Weight", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        _1007->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        _1007->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DimensionCTerminator_Dialog: public Ui_DimensionCTerminator_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIMENSIONCTERMINATOR_DIALOG_H
