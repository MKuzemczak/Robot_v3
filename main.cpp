#include <iostream>
#include <QApplication>

#include "gui.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Robot)

using namespace std;

int main(int argc, char **argv)
{
    qRegisterMetaType<std::string>();
    qRegisterMetaType<Robot>();

    QApplication app (argc, argv);
    GUI gui;

    gui.show();


    return app.exec();
}
