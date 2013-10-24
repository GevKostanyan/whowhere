/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QPointer>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QSettings>

// QtMobility API headers
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
// QtMobility namespace
QTM_USE_NAMESPACE

class Message;
class Daemon: public QObject
{
Q_OBJECT

public:
    Daemon(QObject *parent = 0);
    ~Daemon();

    // WhoWhereDaemon service interface
public:
    // Enables GPS location tracking on
    Q_INVOKABLE void enableDaemon(QVariant enable);
    Q_INVOKABLE QVariant isDaemonEnabled();

    // Enables log
    Q_INVOKABLE void enableDaemonLog(QVariant enable);
    Q_INVOKABLE QVariant isLogEnabled();

    // Does GPS running always
    Q_INVOKABLE void keepGpsRunning(QVariant running);
    Q_INVOKABLE QVariant isGpsRunningEnabled();

    // GPS accuracy in meters
    Q_INVOKABLE void gpsAccuracy(QVariant meters);
    Q_INVOKABLE QVariant getGpsAccuracy();

    // Send your GPS location to phonenumber
    Q_INVOKABLE void sendLocationTo(QVariant number);

    // Store settings to file
    Q_INVOKABLE void storeSettings();

    // Kill daemon service
    Q_INVOKABLE void killDaemon();

signals:
    // Server sends logs to Client
    void daemonLog(QString log);
    // GPS started
    void gpsInitialized();
    // GPS valid location readed
    void gpsLocationReceived();
    // GPS closed
    void gpsClosed();

private:
    void createGPS();
    void deleteGPS();

    void reset();
    void readSettings();
    void saveSettings();


private slots:
    // Signals from Messaging
    void friendAskLocationSMS(QString friendPhoneNumber);

    // Signals from QGeoPositionInfoSource
    void updateTimeout();
    void positionUpdated(QGeoPositionInfo);

    // Log messages into text file
    void log(QString str);

    void startGps();
    
private:
    Message* m_message;
    QPointer<QGeoPositionInfoSource> m_location;
    QGeoPositionInfo m_YougeoPosInfo;
    QFile* m_file;
    QTextStream m_outStream;
    QString m_askingLocation;
    QSettings* m_settings;

    bool m_daemonEnabled;
    bool m_daemonLogEnabled;
    double m_accuracy;
    bool m_keepGpsRunning;

};

#endif // DAEMON_H
