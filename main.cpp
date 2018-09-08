#include <iostream>
#include <QApplication>

#include "gui.h"

using namespace std;

int main(int argc, char **argv)
{
    qRegisterMetaType<std::string>();

    QApplication app (argc, argv);
    GUI gui;

    gui.show();

    return app.exec();
}
