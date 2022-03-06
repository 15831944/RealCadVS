/********************************************************************************
** Form generated from reading UI file 'aboutrealcad_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTREALCAD_DIALOG_H
#define UI_ABOUTREALCAD_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutRealCAD_Dialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextEdit *_100;
    QHBoxLayout *horizontalLayout;
    QPushButton *_1;
    QSpacerItem *horizontalSpacer;
    QPushButton *_106;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *_104;

    void setupUi(QDialog *AboutRealCAD_Dialog)
    {
        if (AboutRealCAD_Dialog->objectName().isEmpty())
            AboutRealCAD_Dialog->setObjectName(QString::fromUtf8("AboutRealCAD_Dialog"));
        AboutRealCAD_Dialog->resize(500, 273);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AboutRealCAD_Dialog->sizePolicy().hasHeightForWidth());
        AboutRealCAD_Dialog->setSizePolicy(sizePolicy);
        AboutRealCAD_Dialog->setMinimumSize(QSize(500, 239));
        AboutRealCAD_Dialog->setMaximumSize(QSize(500, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        AboutRealCAD_Dialog->setFont(font);
        gridLayout = new QGridLayout(AboutRealCAD_Dialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _100 = new QTextEdit(AboutRealCAD_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        _100->setFont(font1);
        _100->setReadOnly(true);

        verticalLayout->addWidget(_100);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        _1 = new QPushButton(AboutRealCAD_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setFont(font);

        horizontalLayout->addWidget(_1);

        horizontalSpacer = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        _106 = new QPushButton(AboutRealCAD_Dialog);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setFont(font);
        _106->setAutoDefault(false);

        horizontalLayout->addWidget(_106);

        horizontalSpacer_2 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        _104 = new QPushButton(AboutRealCAD_Dialog);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setFont(font);

        horizontalLayout->addWidget(_104);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AboutRealCAD_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(AboutRealCAD_Dialog);
    } // setupUi

    void retranslateUi(QDialog *AboutRealCAD_Dialog)
    {
        AboutRealCAD_Dialog->setWindowTitle(QCoreApplication::translate("AboutRealCAD_Dialog", "About RealCAD - CAD for the Mac", nullptr));
        _100->setHtml(QCoreApplication::translate("AboutRealCAD_Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        _1->setText(QCoreApplication::translate("AboutRealCAD_Dialog", "OK", nullptr));
        _106->setText(QCoreApplication::translate("AboutRealCAD_Dialog", "Change network server", nullptr));
        _104->setText(QCoreApplication::translate("AboutRealCAD_Dialog", "Unlicense", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutRealCAD_Dialog: public Ui_AboutRealCAD_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTREALCAD_DIALOG_H
