/********************************************************************
 Copyright © Lin Guannan. 2011-2012. All rights reserved.

 File name:     main.cpp
 First Creator: Lin Guannan
 Data:          2012-5-19
 Description:   entry point for app
 Others:        this file should be keep simple
 History:
     1. Data:            2012-5-19
        Author:          Lin Guannan
        Modification:    First Creation of the file
********************************************************************/
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

