/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "Daemon.h"
#include "Message.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QTimer>


#include <QRemoteServiceRegister>
#include <QServiceManager>

Daemon::Daemon(QObject *parent) :
    QObject(parent)
{
    reset();

    // Daemon settings
    m_settings = new QSettings("nokia","whowheredaemon",this);
    readSettings();

    // For logging messages into text
#ifdef WRITE_LOG_TO_FILE
    QString path("C:\\Data\\whowheredaemon.dat");
    m_file = new QFile(path);
    // Delete old log file
    if (QFile::exists(path)) {
        QFile::remove(path);
    }
    // Open new log
    m_file->open(QIODevice::WriteOnly | QIODevice::Text  | QIODevice::Append);
    m_outStream.setDevice(m_file);
#endif

    // SMS message listening / sending
    m_message = new Message(this);
    QObject::connect(m_message, SIGNAL(friendAskLocationSMS(QString)), this,
                     SLOT(friendAskLocationSMS(QString)));

    // Create GPS
    createGPS();

    // Start GPS
    if (m_daemonEnabled) {
        QTimer::singleShot(5000,this,SLOT(startGps()));
    }

    log("WhoWhereDaemon Started");
}

Daemon::~Daemon()
{
    // Stop GPS
    if (m_location) {
        m_location->stopUpdates();
        delete m_location;
    }
    
#ifdef WRITE_LOG_TO_FILE
    m_file->close();
    delete m_file;
#endif
}

void Daemon::readSettings()
{
    m_daemonEnabled = m_settings->value("daemonEnabled",QVariant(true)).toBool();
    m_daemonLogEnabled = m_settings->value("daemonLogEnabled",QVariant(true)).toBool();
    m_keepGpsRunning = m_settings->value("daemonGpsAlive",QVariant(false)).toBool();
    m_accuracy = m_settings->value("daemonGpsAccuracy",QVariant(20.0)).toDouble();
}

void Daemon::saveSettings()
{
    m_settings->setValue("daemonEnabled",QVariant(m_daemonEnabled));
    m_settings->setValue("daemonLogEnabled",QVariant(m_daemonLogEnabled));
    m_settings->setValue("daemonGpsAlive",QVariant(m_keepGpsRunning));
    m_settings->setValue("daemonGpsAccuracy",QVariant(m_accuracy));
    m_settings->sync();
}

void Daemon::createGPS()
{
    if (!m_location)
    {
        m_location = QGeoPositionInfoSource::createDefaultSource(this);
        m_location->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        m_location->setUpdateInterval(10*1000);
        // System has some positioning source found
        if (m_location) {
            QObject::connect(m_location, SIGNAL(positionUpdated(QGeoPositionInfo)), this,
                             SLOT(positionUpdated(QGeoPositionInfo)));
            QObject::connect(m_location, SIGNAL(updateTimeout()), this, SLOT(updateTimeout()));
        }
        else {
            // System has not any positioning source
            log("Device has not any positioning source. WhoWhereDaemon is exiting");
            // Exit application
            QCoreApplication::exit();
        }
    }
}

void Daemon::deleteGPS()
{
    emit gpsClosed();
    if (m_location) {
        m_location->stopUpdates();
    }
    delete m_location;
    m_location = 0;
    log("GPS stopped");
}

void Daemon::startGps()
{
    emit gpsInitialized();
    createGPS();
    m_location->startUpdates();
    log("GPS started");
}

void Daemon::log(QString str)
{
    qDebug() << str;

    if (m_daemonLogEnabled) {
        emit daemonLog(str);
    }

#ifdef WRITE_LOG_TO_FILE
    str.append("\n");
    m_outStream << str;
    m_outStream.flush();
#endif
}

void Daemon::friendAskLocationSMS(QString friendPhoneNumber)
{
    // Friend asks location
    //log(QString("Send location to number %1 asks your location").arg(friendPhoneNumber));

    if (m_daemonEnabled) {
        // Store requester phone number
        m_askingLocation = friendPhoneNumber;
        // Read your location
        if (!m_YougeoPosInfo.isValid()) {
            startGps();
        } else {
            // You have location, send it
            m_message->sendLocationSMS("RES:", m_YougeoPosInfo, m_askingLocation);
            // Reset position and requester
            reset();
        }
    }
}

void Daemon::updateTimeout()
{
    log("Waiting GPS location...");
}

void Daemon::positionUpdated(QGeoPositionInfo gpsPos)
{
    qreal haMeters = 0;

    if(gpsPos.isValid()) {
        haMeters = gpsPos.attribute(QGeoPositionInfo::HorizontalAccuracy);
        log(QString("Location accuracy is %1 m").arg(haMeters));
    } else {
        log("Location updated, but not valid");
    }

    if (!m_daemonEnabled) {
        deleteGPS();
        return;
    }

    if(gpsPos.isValid()) {
        m_YougeoPosInfo = gpsPos;

        if (haMeters < m_accuracy && haMeters != 0) {

            emit gpsLocationReceived();

            if (!m_keepGpsRunning) {
                deleteGPS();
                log(QString("GPS stopped. Accuracy was %1 m").arg(haMeters));
            }
            // Is there location request?
            if (!m_askingLocation.isEmpty()) {
                // Send location response SMS
                m_message->sendLocationSMS("RES:", m_YougeoPosInfo, m_askingLocation);
                log(QString("SMS sent to number %1").arg(m_askingLocation));
                // Reset position and requester
                reset();
            }
        }
    }
}

void Daemon::reset()
{
    // Reset position and requester
    m_askingLocation = "";
    m_YougeoPosInfo.setCoordinate(QGeoCoordinate());
}

void Daemon::enableDaemon(QVariant enable)
{
    m_daemonEnabled = enable.toBool();

    if (m_daemonEnabled) {
        // Reset position and requester
        reset();
        // Start GPS
        startGps();
        log("Client enables WhoWhereDaemon");
    } else {
        deleteGPS();
        log("Client disables WhoWhereDaemon");
    }

    saveSettings();
}

void Daemon::killDaemon()
{
    log("Client is killing the WhoWhereDaemon");
    QCoreApplication::exit();
}


void Daemon::keepGpsRunning(QVariant running)
{
    m_keepGpsRunning = running.toBool();
    if (m_keepGpsRunning) {
        log("Client keeps GPS running");
    }else {
        log("Client allows GPS to shutdown");
    }

    saveSettings();
}

void Daemon::gpsAccuracy(QVariant meters)
{
    m_accuracy = meters.toDouble();
    log(QString("Client sets GPS accuracy to %1 meters").arg(m_accuracy));
    saveSettings();
}

void Daemon::sendLocationTo(QVariant number)
{
    friendAskLocationSMS(number.toString());
    log(QString("Client sends GPS location to number %1").arg(number.toString()));
}

void Daemon::enableDaemonLog(QVariant enable)
{
    m_daemonLogEnabled = enable.toBool();
    if (m_daemonLogEnabled) {
        log("Client enables WhoWhereDaemon log");
    } else {
        log("Client disables WhoWhereDaemon log");
    }
    saveSettings();
}

QVariant Daemon::isDaemonEnabled()
{
    log("Client asks is daemon enabled");
    return QVariant(m_daemonEnabled);
}

QVariant Daemon::isLogEnabled()
{
    log("Client asks is log enabled");
    return QVariant(m_daemonLogEnabled);
}

QVariant Daemon::isGpsRunningEnabled()
{
    log("Client asks is GPS enabled");
    return QVariant(m_keepGpsRunning);
}

QVariant Daemon::getGpsAccuracy()
{
    log("Client asks GPS accuracy");
    return QVariant(m_accuracy);
}

void Daemon::storeSettings()
{
    saveSettings();
}

