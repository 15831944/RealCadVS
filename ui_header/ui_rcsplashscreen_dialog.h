/********************************************************************************
** Form generated from reading UI file 'rcsplashscreen_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RCSPLASHSCREEN_DIALOG_H
#define UI_RCSPLASHSCREEN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "qwebengineview.h"

QT_BEGIN_NAMESPACE

class Ui_RCSplashScreen_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QWebEngineView *_300;
    QHBoxLayout *horizontalLayout;
    QCheckBox *_200;
    QSpacerItem *horizontalSpacer;
    QPushButton *_1;

    void setupUi(QDialog *RCSplashScreen_Dialog)
    {
        if (RCSplashScreen_Dialog->objectName().isEmpty())
            RCSplashScreen_Dialog->setObjectName(QString::fromUtf8("RCSplashScreen_Dialog"));
        RCSplashScreen_Dialog->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RCSplashScreen_Dialog->sizePolicy().hasHeightForWidth());
        RCSplashScreen_Dialog->setSizePolicy(sizePolicy);
        RCSplashScreen_Dialog->setMinimumSize(QSize(800, 600));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        RCSplashScreen_Dialog->setFont(font);
        verticalLayout = new QVBoxLayout(RCSplashScreen_Dialog);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 10);
        _300 = new QWebEngineView(RCSplashScreen_Dialog);
        _300->setObjectName(QString::fromUtf8("_300"));

        verticalLayout->addWidget(_300);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(50, -1, 50, -1);
        _200 = new QCheckBox(RCSplashScreen_Dialog);
        _200->setObjectName(QString::fromUtf8("_200"));
        _200->setEnabled(true);
        _200->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(_200);

        horizontalSpacer = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _1 = new QPushButton(RCSplashScreen_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setMinimumSize(QSize(90, 32));
        _1->setMaximumSize(QSize(90, 30));

        horizontalLayout->addWidget(_1);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(RCSplashScreen_Dialog);

        QMetaObject::connectSlotsByName(RCSplashScreen_Dialog);
    } // setupUi

    void retranslateUi(QDialog *RCSplashScreen_Dialog)
    {
        RCSplashScreen_Dialog->setWindowTitle(QCoreApplication::translate("RCSplashScreen_Dialog", "Welcome Screen", nullptr));
        _200->setText(QCoreApplication::translate("RCSplashScreen_Dialog", "Always show this dialog", nullptr));
        _1->setText(QCoreApplication::translate("RCSplashScreen_Dialog", "  Close  ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RCSplashScreen_Dialog: public Ui_RCSplashScreen_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RCSPLASHSCREEN_DIALOG_H
