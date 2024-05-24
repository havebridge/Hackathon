/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QWidget *widget;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *LoginLineEdit;
    QLineEdit *PasswordLineEdit;
    QPushButton *SignUpToButton;
    QPushButton *SignInToButton;

    void setupUi(QWidget *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName("mainWindowClass");
        mainWindowClass->resize(727, 400);
        mainWindowClass->setStyleSheet(QString::fromUtf8("background-color: gray;"));
        widget = new QWidget(mainWindowClass);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 741, 411));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setStyleSheet(QString::fromUtf8("background-color: rgba(28,28,28,150);\n"
"setalignment: center\n"
""));
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(225, 160, 81, 21));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(13);
        font.setBold(true);
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color: white;\n"
"background-color: transparent;"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(225, 100, 51, 21));
        label_2->setFont(font);
        label_2->setAutoFillBackground(false);
        label_2->setStyleSheet(QString::fromUtf8("color: white;\n"
"background-color: transparent;\n"
""));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        LoginLineEdit = new QLineEdit(widget);
        LoginLineEdit->setObjectName("LoginLineEdit");
        LoginLineEdit->setGeometry(QRect(225, 130, 280, 22));
        LoginLineEdit->setFont(font);
        LoginLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgba(14,14,14,150);\n"
"color: white;\n"
"border-radius: 5px;"));
        PasswordLineEdit = new QLineEdit(widget);
        PasswordLineEdit->setObjectName("PasswordLineEdit");
        PasswordLineEdit->setGeometry(QRect(225, 190, 280, 22));
        PasswordLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgba(14,14,14,150);\n"
"color: white;\n"
"border-radius: 5px;"));
        SignUpToButton = new QPushButton(widget);
        SignUpToButton->setObjectName("SignUpToButton");
        SignUpToButton->setGeometry(QRect(225, 230, 121, 24));
        SignUpToButton->setFont(font);
        SignUpToButton->setStyleSheet(QString::fromUtf8("background-color: rgb(44,168,60);\n"
"border-radius: 5px;"));
        SignInToButton = new QPushButton(widget);
        SignInToButton->setObjectName("SignInToButton");
        SignInToButton->setGeometry(QRect(380, 230, 121, 24));
        SignInToButton->setFont(font);
        SignInToButton->setStyleSheet(QString::fromUtf8("background-color: rgb(44,168,60);\n"
"border-radius: 5px;"));

        retranslateUi(mainWindowClass);

        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QWidget *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QCoreApplication::translate("mainWindowClass", "Prosoft", nullptr));
        label_3->setText(QCoreApplication::translate("mainWindowClass", "Password", nullptr));
        label_2->setText(QCoreApplication::translate("mainWindowClass", "Login", nullptr));
        SignUpToButton->setText(QCoreApplication::translate("mainWindowClass", "Sign Up", nullptr));
        SignInToButton->setText(QCoreApplication::translate("mainWindowClass", "Sign In", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
