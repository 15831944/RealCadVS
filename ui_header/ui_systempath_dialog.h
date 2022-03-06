/********************************************************************************
** Form generated from reading UI file 'systempath_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMPATH_DIALOG_H
#define UI_SYSTEMPATH_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SystemPath_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *_100;
    QHBoxLayout *horizontalLayout;
    QPushButton *_101;
    QPushButton *_102;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;
    QPushButton *_2;

    void setupUi(QDialog *SystemPath_Dialog)
    {
        if (SystemPath_Dialog->objectName().isEmpty())
            SystemPath_Dialog->setObjectName(QString::fromUtf8("SystemPath_Dialog"));
        SystemPath_Dialog->resize(550, 300);
        SystemPath_Dialog->setMinimumSize(QSize(550, 300));
        verticalLayout = new QVBoxLayout(SystemPath_Dialog);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _100 = new QTreeWidget(SystemPath_Dialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        _100->setHeaderItem(__qtreewidgetitem);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setEditTriggers(QAbstractItemView::SelectedClicked);
        _100->setTabKeyNavigation(true);
        _100->setProperty("showDropIndicator", QVariant(false));
        _100->header()->setVisible(false);
        _100->header()->setDefaultSectionSize(200);

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        _101 = new QPushButton(SystemPath_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setAutoDefault(false);

        horizontalLayout->addWidget(_101);

        _102 = new QPushButton(SystemPath_Dialog);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setAutoDefault(false);

        horizontalLayout->addWidget(_102);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(SystemPath_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));

        horizontalLayout->addWidget(_1);

        _2 = new QPushButton(SystemPath_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setAutoDefault(false);

        horizontalLayout->addWidget(_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SystemPath_Dialog);

        _101->setDefault(false);
        _1->setDefault(true);


        QMetaObject::connectSlotsByName(SystemPath_Dialog);
    } // setupUi

    void retranslateUi(QDialog *SystemPath_Dialog)
    {
        SystemPath_Dialog->setWindowTitle(QCoreApplication::translate("SystemPath_Dialog", "File Locations ", nullptr));
        _101->setText(QCoreApplication::translate("SystemPath_Dialog", "Modify...", nullptr));
        _102->setText(QCoreApplication::translate("SystemPath_Dialog", "Reset", nullptr));
        _1->setText(QCoreApplication::translate("SystemPath_Dialog", "OK", nullptr));
        _2->setText(QCoreApplication::translate("SystemPath_Dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SystemPath_Dialog: public Ui_SystemPath_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMPATH_DIALOG_H
