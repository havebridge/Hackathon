#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainWindow.h"

#include <QMessageBox>
#include <QINputDialog>
#include <QVBoxLayout>

#include "client.h"


class mainWindow : public QWidget
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void SignUpToButton_clicked();
    void SignInToButton_clicked();

private:
    Ui::mainWindowClass* ui;

    Client client;
};
