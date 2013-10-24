/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1
import QtMobility.serviceframework 1.1

Item {
    id: container
    width: 100
    height: 62


    // Listen WhoWhereDaemon Service logs
    Connections {
        target: main.daemonService
        ignoreUnknownSignals: true
        onGpsInitialized: {
            noGpsImage.opacity = 0
            gpsImage.opacity = 1
            rotAnim.restart();
        }
        onGpsLocationReceived: {
            noGpsImage.opacity = 0
            gpsImage.opacity = 1
            rotAnim.stop();
        }
        onGpsClosed: {
            noGpsImage.opacity = 1
            gpsImage.opacity = 0
            rotAnim.stop();
        }
    }

    RotationAnimation {
        id: rotAnim
        loops: Animation.Infinite
        duration: 4000
        from: 0
        to: 359
        target: container
        property: "rotation"
    }


    Image {
        id: noGpsImage
        opacity: 1
        anchors.fill: parent
        smooth:  true
        source: "qrc:/images/gpsOff.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: gpsImage
        opacity: 0
        anchors.fill: parent
        smooth:  true
        source: "qrc:/images/gpsOn.png"
        fillMode: Image.PreserveAspectFit
    }


}
