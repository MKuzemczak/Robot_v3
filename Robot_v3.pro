TARGET = Robot_v3

QT = core gui

QMAKE_CXXFLAGS += -pedantic

CONFIG += c++14 strict_c++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    gui.cpp \
    main.cpp \
    serialport.cpp \
    thread.cpp \
    serialcommunicator.cpp

HEADERS += \
    gui.h \
    serialport.h \
    eigenaddons.h \
    lista.h \
    comflags.h \
    thread.h \
    serialcommunicator.h
