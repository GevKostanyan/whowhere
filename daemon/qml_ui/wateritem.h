/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef WATERITEM_H
#define WATERITEM_H

#include <QDeclarativeItem>
#include "watergen.h"

class WaterItem : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit WaterItem(QDeclarativeItem *parent = 0);

    virtual ~WaterItem();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    QRectF boundingRect() const;

    Q_INVOKABLE void startWater(QVariant updateSpeed);
    Q_INVOKABLE void setWaterUpdateSpeed(QVariant updateSpeed);
    Q_INVOKABLE void pauseWater(QVariant enable);

private slots:
    void timeout();
    void delayedWaterStart();

private:
    WaterGen* m_waterGen;
    QImage m_waterImage;
    QImage m_targetImage;
    QTimer* m_paintTimer;
    bool m_paused;
    int m_updateSpeed;
};

QML_DECLARE_TYPE(WaterItem)

#endif // WATERITEM_H
