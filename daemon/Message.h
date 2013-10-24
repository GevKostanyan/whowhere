/*
 * Copyright (c) 2011 Nokia Corporation.
 */


#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

// QtMobility API headers
// Messaging
#include <QMessage>
#include <QMessageManager>
#include <QMessageService>

// Location
#include <QGeoPositionInfo>

// QtMobility namespace
QTM_USE_NAMESPACE

class Message: public QObject
{
Q_OBJECT

public:
    Message(QObject *parent = 0);
    ~Message();
    bool sendLocationSMS(QString typeStr, QGeoPositionInfo& position, QString phoneNumber);
    
private:
    void processIncomingSMS(const QMessageId& id);
    
public slots:
    // Listening QMessageManager
    void messageAdded(const QMessageId &id,
        const QMessageManager::NotificationFilterIdSet &matchingFilterIds);

signals:
    void friendAskLocationSMS(QString friendPhoneNumber);

private:
    QMessageService* m_service;
    QMessageManager* m_manager;
    QMessageManager::NotificationFilterIdSet m_notifFilterSet;
    QMessageId m_messageId;
};

#endif // MESSAGE_H
