#define _CRT_SECURE_NO_DEPRECATE
#include "mainWindow.h"
#include <QtWidgets/QApplication>

#include "client.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow w;   
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    w.show();
    return a.exec();
}