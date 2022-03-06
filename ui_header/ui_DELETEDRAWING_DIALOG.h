/********************************************************************************
** Form generated from reading UI file 'DELETEDRAWING_DIALOG.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEDRAWING_DIALOG_H
#define UI_DELETEDRAWING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DeleteDrawing_Dialog
{
public:
    QLabel *label1;
    QPushButton *_2;
    QPushButton *_101;
    QListWidget *_100;

    void setupUi(QDialog *DeleteDrawing_Dialog)
    {
        if (DeleteDrawing_Dialog->objectName().isEmpty())
            DeleteDrawing_Dialog->setObjectName(QString::fromUtf8("DeleteDrawing_Dialog"));
        DeleteDrawing_Dialog->resize(321, 263);
        QFont font;
        font.setPointSize(10);
        DeleteDrawing_Dialog->setFont(font);
        label1 = new QLabel(DeleteDrawing_Dialog);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(20, 10, 331, 20));
        label1->setFont(font);
        label1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _2 = new QPushButton(DeleteDrawing_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(220, 230, 75, 23));
        _2->setAutoDefault(false);
        _101 = new QPushButton(DeleteDrawing_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(130, 230, 75, 23));
        _101->setAutoDefault(true);
        _100 = new QListWidget(DeleteDrawing_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(12, 34, 301, 185));
        _100->setMinimumSize(QSize(301, 185));
        _100->setMaximumSize(QSize(301, 185));
        _100->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        _100->setSelectionMode(QAbstractItemView::ExtendedSelection);

        retranslateUi(DeleteDrawing_Dialog);

        _101->setDefault(true);


        QMetaObject::connectSlotsByName(DeleteDrawing_Dialog);
    } // setupUi

    void retranslateUi(QDialog *DeleteDrawing_Dialog)
    {
        DeleteDrawing_Dialog->setWindowTitle(QCoreApplication::translate("DeleteDrawing_Dialog", "Delete layout drawing", nullptr));
        label1->setText(QCoreApplication::translate("DeleteDrawing_Dialog", "Select the layout drawing to delete:", nullptr));
        _2->setText(QCoreApplication::translate("DeleteDrawing_Dialog", "Cancel", nullptr));
        _101->setText(QCoreApplication::translate("DeleteDrawing_Dialog", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeleteDrawing_Dialog: public Ui_DeleteDrawing_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEDRAWING_DIALOG_H
