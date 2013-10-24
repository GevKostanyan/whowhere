/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1
import QtMobility.serviceframework 1.1

Rectangle {
    id: container
    color: "white"
    smooth: true
    border.width: 1
    border.color: "gray"
    radius: 8

    property string logString;

    gradient: Gradient {
             GradientStop { position: 0.0; color: "#bab7b0" }
             GradientStop { position: 1.0; color: "#edede5" }
         }

    // Listen WhoWhereDaemon Service logs
    Connections {
        target: main.daemonService
        ignoreUnknownSignals: true
        onDaemonLog: {
            //console.log("From Daemon: " + log);
            logString += log + "\n";
            textId.text = logString;
            flickable.contentY += tmpText.paintedHeight
        }
    }

    Text {
        id: tmpText
        opacity: 0
        font.pointSize: textId.font.pointSize
        font.family: "Courier"
        text: "TextHeight"
        smooth: true
    }

    Flickable {
        id: flickable
        anchors {
            fill: parent
        }
        contentWidth: width
        contentHeight: textId.height
        clip: true

        Text {
            id: textId

            width: parent.width - 10

            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 5
            anchors.bottomMargin: 5

            wrapMode: Text.WordWrap
            //color: "#d8dcdf"
            color: "black"
            font.pointSize: 6
            font.family: "Courier"
            text: ""
            smooth: true
        }
    }

}
