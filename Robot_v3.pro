TARGET = Robot_v3

QT = core gui

QMAKE_CXXFLAGS +=

CONFIG += c++14 strict_c++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    gui.cpp \
    main.cpp \
    serialport.cpp \
    thread.cpp \
    serialcommunicator.cpp \
    joint.cpp \
    robot.cpp \
    baseaction.cpp \
    straightlinemovaction.cpp \
    freemovaction.cpp \
    setsinglejointaction.cpp \
    consttcporientaction.cpp \
    actionmanager.cpp \
    program.cpp \
    gripperaction.cpp

HEADERS += \
    gui.h \
    serialport.h \
    eigenaddons.h \
    lista.h \
    comflags.h \
    thread.h \
    serialcommunicator.h \
    joint.h \
    robot.h \
    baseaction.h \
    actionmanager.h \
    program.h
