/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "Message.h"
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>

Message::Message(QObject *parent) :
    QObject(parent)
{
    // QMessageService class provides the interface for requesting messaging service operations
    m_service = new QMessageService(this);

    // QMessageManager class represents the main interface for storage and
    // retrieval of messages, folders and accounts in the system message store
    m_manager = new QMessageManager(this);
    QObject::connect(m_manager,
        SIGNAL(messageAdded(const QMessageId&,const QMessageManager::NotificationFilterIdSet&)),
        this, SLOT(messageAdded(const QMessageId&,const QMessageManager::NotificationFilterIdSet&)));

    // Register SMS in inbox folder notificationfilter
    m_notifFilterSet.insert(m_manager->registerNotificationFilter(QMessageFilter::byStandardFolder(
        QMessage::InboxFolder)));

}

Message::~Message()
{
}

void Message::messageAdded(const QMessageId& id,
    const QMessageManager::NotificationFilterIdSet& matchingFilterIds)
{
    if (matchingFilterIds.contains(m_notifFilterSet))
        processIncomingSMS(id);
}

void Message::processIncomingSMS(const QMessageId& id)
{
    QMessage message = m_manager->message(id);

    // Handle only SMS messages
    if (message.type() != QMessage::Sms)
        return;

    QString txt = message.textContent();
    if (txt.length() > 10) {
        if (txt.mid(0, 4).contains("REQ:", Qt::CaseSensitive)) {
            // Your friend send location request to you
            QMessageAddress from = message.from();
            if (from.type() == QMessageAddress::Phone) {
                emit friendAskLocationSMS(from.addressee());
            }
            // Remove message from inbox
            m_manager->removeMessage(id);
        }
    }
}

bool Message::sendLocationSMS(QString typeStr, QGeoPositionInfo& position, QString phoneNumber)
{
    // Send SMS
    if (!position.isValid()) {
        return false;
    }

    QGeoCoordinate coordinate = position.coordinate();

    QMessage smsMessage;
    smsMessage.setType(QMessage::Sms);
    smsMessage.setParentAccountId(QMessageAccount::defaultAccount(QMessage::Sms));    
    smsMessage.setTo(QMessageAddress(QMessageAddress::Phone, phoneNumber));

    QString bodyText;
    bodyText += typeStr;
    bodyText += QString().setNum(coordinate.latitude(),'g',8);
    bodyText += " ";
    bodyText += QString().setNum(coordinate.longitude(),'g',8);
    smsMessage.setBody(bodyText);

    // Send SMS
    return m_service->send(smsMessage);
}

