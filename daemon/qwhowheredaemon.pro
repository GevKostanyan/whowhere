
# Copyright (c) 2011 Nokia Corporation

TEMPLATE = app
TARGET = qwhowheredaemon
QT += core
QT -= gui

VERSION = 1.0.1

# Does not have Symbian icon
CONFIG += no_icon

# For QtMobility	
CONFIG += mobility
MOBILITY = location messaging serviceframework

HEADERS += Daemon.h \
    Message.h
SOURCES += Daemon.cpp \
    Message.cpp \
    main.cpp

OTHER_FILES += wwdaemonservice.xml

# Symbian
symbian { 
    load(data_caging_paths)

    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x3000000

    TARGET.UID3 = 0xE2E04FD2

    # For QtMobility
    # NOTE: ReadDeviceData and WriteDeviceData needs developer signing
    TARGET.CAPABILITY = NetworkServices \
        Location \
        ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment \
        ReadDeviceData \
        WriteDeviceData

    # Daemon as Qt Mobility Service FW service
    xmlServiceFile.sources = wwdaemonservice.xml
    xmlServiceFile.path = c:$$APP_RESOURCE_DIR/xmldata
    DEPLOYMENT += xmlServiceFile

    # Writes log into C:\Data\whowheredaemon.dat file
    #DEFINES += WRITE_LOG_TO_FILE

}

