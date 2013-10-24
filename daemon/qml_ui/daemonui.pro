
# Copyright (c) 2011 Nokia Corporation.

TEMPLATE = app
TARGET = whowheredaemonui

VERSION = 1.0.1

QT += declarative

SOURCES +=  main.cpp \
            watergen.cpp \
    wateritem.cpp

HEADERS += watergen.h \
    wateritem.h
    
RESOURCES += resources.qrc
    
OTHER_FILES += main.qml \
    DaemonLog.qml \
    ContactsView.qml \
    InfoView.qml \
    Switch.qml \
    KnobDial.qml \
    Button.qml \
    JavaScript.js \
    GpsIcon.qml

# For QtMobility
CONFIG += mobility
MOBILITY = serviceframework sysinfo

ICON = icon.svg

symbian: {
    # Because orientation lock
    LIBS += -lcone -leikcore -lavkon
    TARGET.UID3 = 0xE2E04FD3
    TARGET = WhoWhereDaemonUI
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x20000 0x6000000

    # Self-signing capabilities
    TARGET.CAPABILITY += NetworkServices \
        ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment

    # Sound
    sound.sources = "sounds//13564__acclivity__GullsByTheSea.wav"
    sound.path = !:/private/E2E04FD3

    # The backup and restore functionality
    backup.sources = backup_registration.xml
    backup.path = !:/private/E2E04FD3
    DEPLOYMENT += sound backup

}
