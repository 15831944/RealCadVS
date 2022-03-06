/********************************************************************************
** Form generated from reading UI file 'rccombolist.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCCOMBOLIST_H
#define UI_RCCOMBOLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RCComboList
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QListWidget *listWidget;

    void setupUi(QWidget *RCComboList)
    {
        if (RCComboList->objectName().isEmpty())
            RCComboList->setObjectName(QString::fromUtf8("RCComboList"));
        RCComboList->resize(400, 300);
        verticalLayoutWidget = new QWidget(RCComboList);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1, 1, 399, 299));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        listWidget = new QListWidget(verticalLayoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        listWidget->setUniformItemSizes(true);
        listWidget->setSortingEnabled(true);

        verticalLayout->addWidget(listWidget);


        retranslateUi(RCComboList);

        QMetaObject::connectSlotsByName(RCComboList);
    } // setupUi

    void retranslateUi(QWidget *RCComboList)
    {
        RCComboList->setWindowTitle(QCoreApplication::translate("RCComboList", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RCComboList: public Ui_RCComboList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCCOMBOLIST_H
