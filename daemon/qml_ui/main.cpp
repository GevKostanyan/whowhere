/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QtGui>
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDebug>

// Lock Symbian orientation
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

#include "wateritem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Register WaterItem custom QML item
    qmlRegisterType<WaterItem>("CustomItems", 1, 0, "WaterItem");

    // Lock Symbian orientation
#ifdef Q_OS_SYMBIAN
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAP_IGNORE( 
    if (appUi) {
        appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
    }
    );
#endif

    QDeclarativeView* view = new QDeclarativeView();

    // Tell the QML side the path of where app exist
    view->rootContext()->setContextProperty("appFolder",view->engine()->baseUrl().toString());

    view->setSource(QUrl("qrc:/main.qml"));
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    // For Qt.quit() from QML
    QObject::connect((QObject*)view->engine(), SIGNAL(quit()), &a, SLOT(quit()));

    view->showFullScreen();
    int ret = a.exec();
    delete view;
    return ret;
}
