/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QtCore>
#include <QCoreApplication>

#include "Daemon.h"

#include <QRemoteServiceRegister>
#include <QServiceManager>


int main(int argc, char *argv[])
{
    // Application without UI is QCoreApplication not QApplication
    QCoreApplication a(argc, argv);

    // Create daemon
    Daemon daemon;

    // Create service
    // Create, register and publish IPC based service object
    const QString serviceName("WhoWhereDaemonService");
    const QString interfaceName("com.nokia.qt.examples.qwhowheredaemon");
    const QString serviceVersion("1.0");

    QtMobility::QServiceManager manager;

    // Remove old service
    manager.removeService(serviceName);

    // Add service
    bool addServiceOk = manager.addService("c:\\resource\\apps\\xmldata\\wwdaemonservice.xml");
    Q_ASSERT(addServiceOk);

    // Entry
    QtMobility::QRemoteServiceRegister serviceRegister;
    QtMobility::QRemoteServiceRegister::Entry entry = serviceRegister.createEntry<Daemon>(serviceName, interfaceName, serviceVersion);

    // Publish
    serviceRegister.publishEntries("qwhowheredaemon");

    // Keep service running
    serviceRegister.setQuitOnLastInstanceClosed(false);

    return a.exec();
}
