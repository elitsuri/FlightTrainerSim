// File: main.cpp
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("FlightTrainerSim");
    app.setOrganizationName("Defense Aviation Systems");
    app.setApplicationVersion("1.0.0");
    
    MainWindow mainWindow;
    mainWindow.showMaximized();
    
    return app.exec();
}
