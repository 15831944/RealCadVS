/********************************************************************************
** Form generated from reading UI file 'LAYERGROUP_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERGROUP_DIALOG_H
#define UI_LAYERGROUP_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_LayerGroup_Dialog
{
public:
    QComboBox *_100;
    QLabel *label;
    QPushButton *_300;
    QPushButton *_400;
    QPushButton *_1;
    QPushButton *_2;
    QTableView *_200;

    void setupUi(QDialog *LayerGroup_Dialog)
    {
        if (LayerGroup_Dialog->objectName().isEmpty())
            LayerGroup_Dialog->setObjectName(QString::fromUtf8("LayerGroup_Dialog"));
        LayerGroup_Dialog->resize(400, 316);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LayerGroup_Dialog->sizePolicy().hasHeightForWidth());
        LayerGroup_Dialog->setSizePolicy(sizePolicy);
        LayerGroup_Dialog->setMinimumSize(QSize(400, 316));
        LayerGroup_Dialog->setMaximumSize(QSize(400, 16777215));
        _100 = new QComboBox(LayerGroup_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(114, 16, 265, 22));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        _100->setFont(font);
        _100->setEditable(true);
        label = new QLabel(LayerGroup_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(18, 20, 93, 16));
        label->setFont(font);
        _300 = new QPushButton(LayerGroup_Dialog);
        _300->setObjectName(QString::fromUtf8("_300"));
        _300->setGeometry(QRect(82, 250, 75, 27));
        QFont font1;
        font1.setPointSize(11);
        _300->setFont(font1);
        _400 = new QPushButton(LayerGroup_Dialog);
        _400->setObjectName(QString::fromUtf8("_400"));
        _400->setGeometry(QRect(242, 248, 75, 27));
        _400->setFont(font1);
        _1 = new QPushButton(LayerGroup_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(118, 286, 75, 27));
        _1->setFont(font1);
        _2 = new QPushButton(LayerGroup_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(204, 286, 75, 27));
        _2->setFont(font1);
        _2->setAutoDefault(false);
        _200 = new QTableView(LayerGroup_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setGeometry(QRect(14, 44, 369, 199));
        _200->setFont(font);
        _200->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _200->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _200->setDragDropOverwriteMode(false);
        _200->setSelectionMode(QAbstractItemView::MultiSelection);
        _200->setSelectionBehavior(QAbstractItemView::SelectRows);
        _200->setShowGrid(false);
        _200->setWordWrap(false);
        _200->horizontalHeader()->setHighlightSections(false);
        _200->verticalHeader()->setVisible(false);
        _200->verticalHeader()->setHighlightSections(false);

        retranslateUi(LayerGroup_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(LayerGroup_Dialog);
    } // setupUi

    void retranslateUi(QDialog *LayerGroup_Dialog)
    {
        LayerGroup_Dialog->setWindowTitle(QCoreApplication::translate("LayerGroup_Dialog", "Layer Palettes", nullptr));
        label->setText(QCoreApplication::translate("LayerGroup_Dialog", "Palette Name", nullptr));
        _300->setText(QCoreApplication::translate("LayerGroup_Dialog", "Apply", nullptr));
        _400->setText(QCoreApplication::translate("LayerGroup_Dialog", "Delete", nullptr));
        _1->setText(QCoreApplication::translate("LayerGroup_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("LayerGroup_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LayerGroup_Dialog: public Ui_LayerGroup_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERGROUP_DIALOG_H
