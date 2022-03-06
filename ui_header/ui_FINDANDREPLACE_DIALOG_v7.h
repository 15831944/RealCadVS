/********************************************************************************
** Form generated from reading UI file 'FINDANDREPLACE_DIALOG_v7.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDANDREPLACE_DIALOG_V7_H
#define UI_FINDANDREPLACE_DIALOG_V7_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FindAndReplace_Dialog
{
public:
    QLineEdit *_100;
    QLabel *label_44;
    QLineEdit *_101;
    QLabel *label_45;
    QPushButton *_1000;
    QPushButton *_1001;
    QGroupBox *groupBox;
    QCheckBox *_103;
    QCheckBox *_107;
    QCheckBox *_105;
    QCheckBox *_108;
    QCheckBox *_109;
    QGroupBox *groupBox_2;
    QCheckBox *_102;
    QCheckBox *_104;
    QCheckBox *_106;
    QPushButton *_2;
    QPushButton *_1;

    void setupUi(QDialog *FindAndReplace_Dialog)
    {
        if (FindAndReplace_Dialog->objectName().isEmpty())
            FindAndReplace_Dialog->setObjectName(QString::fromUtf8("FindAndReplace_Dialog"));
        FindAndReplace_Dialog->resize(563, 300);
        FindAndReplace_Dialog->setMinimumSize(QSize(563, 300));
        FindAndReplace_Dialog->setMaximumSize(QSize(563, 300));
        QFont font;
        font.setPointSize(10);
        FindAndReplace_Dialog->setFont(font);
        _100 = new QLineEdit(FindAndReplace_Dialog);
        _100->setObjectName(QString::fromUtf8("_100"));
        _100->setGeometry(QRect(100, 10, 361, 20));
        label_44 = new QLabel(FindAndReplace_Dialog);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(20, 10, 77, 20));
        label_44->setFont(font);
        label_44->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _101 = new QLineEdit(FindAndReplace_Dialog);
        _101->setObjectName(QString::fromUtf8("_101"));
        _101->setGeometry(QRect(100, 40, 361, 20));
        label_45 = new QLabel(FindAndReplace_Dialog);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(20, 40, 75, 20));
        label_45->setFont(font);
        label_45->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        _1000 = new QPushButton(FindAndReplace_Dialog);
        _1000->setObjectName(QString::fromUtf8("_1000"));
        _1000->setGeometry(QRect(470, 10, 81, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Lucida Grande"));
        font1.setPointSize(10);
        _1000->setFont(font1);
        _1001 = new QPushButton(FindAndReplace_Dialog);
        _1001->setObjectName(QString::fromUtf8("_1001"));
        _1001->setGeometry(QRect(470, 40, 81, 21));
        _1001->setFont(font1);
        groupBox = new QGroupBox(FindAndReplace_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(70, 70, 411, 91));
        _103 = new QCheckBox(groupBox);
        _103->setObjectName(QString::fromUtf8("_103"));
        _103->setEnabled(true);
        _103->setGeometry(QRect(20, 20, 131, 21));
        _103->setLayoutDirection(Qt::RightToLeft);
        _103->setCheckable(true);
        _103->setChecked(false);
        _103->setTristate(false);
        _107 = new QCheckBox(groupBox);
        _107->setObjectName(QString::fromUtf8("_107"));
        _107->setEnabled(true);
        _107->setGeometry(QRect(12, 40, 139, 21));
        _107->setLayoutDirection(Qt::RightToLeft);
        _107->setCheckable(true);
        _107->setChecked(false);
        _107->setTristate(false);
        _105 = new QCheckBox(groupBox);
        _105->setObjectName(QString::fromUtf8("_105"));
        _105->setEnabled(true);
        _105->setGeometry(QRect(20, 60, 131, 21));
        _105->setLayoutDirection(Qt::RightToLeft);
        _105->setCheckable(true);
        _105->setChecked(false);
        _105->setTristate(false);
        _108 = new QCheckBox(groupBox);
        _108->setObjectName(QString::fromUtf8("_108"));
        _108->setEnabled(true);
        _108->setGeometry(QRect(206, 20, 177, 21));
        _108->setLayoutDirection(Qt::RightToLeft);
        _108->setCheckable(true);
        _108->setChecked(false);
        _108->setTristate(false);
        _109 = new QCheckBox(groupBox);
        _109->setObjectName(QString::fromUtf8("_109"));
        _109->setEnabled(true);
        _109->setGeometry(QRect(210, 40, 173, 21));
        _109->setLayoutDirection(Qt::RightToLeft);
        _109->setCheckable(true);
        _109->setChecked(false);
        _109->setTristate(false);
        groupBox_2 = new QGroupBox(FindAndReplace_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(70, 170, 411, 71));
        _102 = new QCheckBox(groupBox_2);
        _102->setObjectName(QString::fromUtf8("_102"));
        _102->setEnabled(true);
        _102->setGeometry(QRect(20, 22, 131, 21));
        _102->setLayoutDirection(Qt::RightToLeft);
        _102->setCheckable(true);
        _102->setChecked(false);
        _102->setTristate(false);
        _104 = new QCheckBox(groupBox_2);
        _104->setObjectName(QString::fromUtf8("_104"));
        _104->setEnabled(true);
        _104->setGeometry(QRect(20, 42, 131, 21));
        _104->setLayoutDirection(Qt::RightToLeft);
        _104->setCheckable(true);
        _104->setChecked(false);
        _104->setTristate(false);
        _106 = new QCheckBox(groupBox_2);
        _106->setObjectName(QString::fromUtf8("_106"));
        _106->setEnabled(true);
        _106->setGeometry(QRect(160, 22, 223, 21));
        _106->setLayoutDirection(Qt::RightToLeft);
        _106->setCheckable(true);
        _106->setChecked(false);
        _106->setTristate(false);
        _2 = new QPushButton(FindAndReplace_Dialog);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setGeometry(QRect(470, 250, 75, 23));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Lucida Grande"));
        font2.setPointSize(11);
        _2->setFont(font2);
        _2->setAutoDefault(false);
        _1 = new QPushButton(FindAndReplace_Dialog);
        _1->setObjectName(QString::fromUtf8("_1"));
        _1->setGeometry(QRect(380, 250, 75, 23));
        _1->setFont(font1);
        QWidget::setTabOrder(_100, _1000);
        QWidget::setTabOrder(_1000, _101);
        QWidget::setTabOrder(_101, _1001);
        QWidget::setTabOrder(_1001, _109);
        QWidget::setTabOrder(_109, _103);
        QWidget::setTabOrder(_103, _105);
        QWidget::setTabOrder(_105, _107);
        QWidget::setTabOrder(_107, _108);
        QWidget::setTabOrder(_108, _102);
        QWidget::setTabOrder(_102, _104);
        QWidget::setTabOrder(_104, _106);
        QWidget::setTabOrder(_106, _1);
        QWidget::setTabOrder(_1, _2);

        retranslateUi(FindAndReplace_Dialog);

        _1->setDefault(true);


        QMetaObject::connectSlotsByName(FindAndReplace_Dialog);
    } // setupUi

    void retranslateUi(QDialog *FindAndReplace_Dialog)
    {
        FindAndReplace_Dialog->setWindowTitle(QCoreApplication::translate("FindAndReplace_Dialog", "Find and replace", nullptr));
        label_44->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Text to find", nullptr));
        label_45->setText(QCoreApplication::translate("FindAndReplace_Dialog", "New text", nullptr));
        _1000->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Browse...", nullptr));
        _1001->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Browse...", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FindAndReplace_Dialog", "Entity search options", nullptr));
        _103->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Text", nullptr));
        _107->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Component names", nullptr));
        _105->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Entity attributes", nullptr));
        _108->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Group class", nullptr));
        _109->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Group instance", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FindAndReplace_Dialog", "Options", nullptr));
        _102->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Replace text", nullptr));
        _104->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Match case", nullptr));
        _106->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Fit window to selected entities", nullptr));
        _2->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Cancel", nullptr));
        _1->setText(QCoreApplication::translate("FindAndReplace_Dialog", "Find", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindAndReplace_Dialog: public Ui_FindAndReplace_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDANDREPLACE_DIALOG_V7_H
