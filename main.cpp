#include <iostream>
#include <QApplication>

#include "gui.h"

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Robot)
Q_DECLARE_METATYPE(ActionType)

using namespace std;

int main(int argc, char **argv)
{
    qRegisterMetaType<std::string>();
    qRegisterMetaType<Robot>();
    qRegisterMetaType<ActionType>();

    QApplication app (argc, argv);
    GUI gui;

    gui.show();


    return app.exec();
}
