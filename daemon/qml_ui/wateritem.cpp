#include "wateritem.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>

#define WATERBLOCK_SIZE 256


WaterItem::WaterItem(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    m_paused = false;

    m_waterGen = new WaterGen();
    m_waterImage = QImage(":/images/reflect.png");
    m_targetImage = QImage(WATERBLOCK_SIZE,WATERBLOCK_SIZE, QImage::Format_ARGB32_Premultiplied);

    m_paintTimer = new QTimer(this);
    QObject::connect(m_paintTimer,SIGNAL(timeout()),this, SLOT(timeout()));
    m_updateSpeed = 80; // Default update speed
    QTimer::singleShot(2000,this,SLOT(delayedWaterStart()));
}

WaterItem::~WaterItem()
{
    if (m_paintTimer)
        m_paintTimer->stop();

    delete m_waterGen;
}

void WaterItem::delayedWaterStart()
{
    startWater(m_updateSpeed);
}

QRectF WaterItem::boundingRect() const
{
    return QDeclarativeItem::boundingRect();
}


void WaterItem::startWater(QVariant updateSpeed)
{
    m_updateSpeed = updateSpeed.toInt();
    m_paused = false;
    m_paintTimer->stop();
    m_paintTimer->start(m_updateSpeed);
}

void WaterItem::setWaterUpdateSpeed(QVariant updateSpeed)
{
    m_updateSpeed = updateSpeed.toInt();
    if (m_paintTimer) {
        m_paintTimer->setInterval(m_updateSpeed);
    }
}


void WaterItem::pauseWater(QVariant enable)
{
    m_paused = enable.toBool();
}

void WaterItem::timeout()
{
    if (this->opacity()!=0 && !m_paused)
    {
        m_waterGen->generate(m_targetImage,m_waterImage, 0.002f);
        update();
    }
}

void WaterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, false);

    if (m_targetImage.byteCount()>0)
    {
        painter->setClipRect(boundingRect());
        int y=0;
        int x=0;
        while (y<height()) {
            while (x<width()) {
                painter->drawImage(x,y,m_targetImage);
                x+=WATERBLOCK_SIZE;
            }
            y+=WATERBLOCK_SIZE;
            x=0;
        }
    }
}
