/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName(QStringLiteral("QtMainWindow"));
        QtMainWindow->resize(400, 300);
        centralWidget = new QWidget(QtMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 24));
        QtMainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtMainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtMainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtMainWindow->setStatusBar(statusBar);

        retranslateUi(QtMainWindow);

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QApplication::translate("QtMainWindow", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
