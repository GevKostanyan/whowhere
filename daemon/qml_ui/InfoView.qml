/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1

Item {
    id: container
    anchors.fill: parent
    anchors.centerIn: parent
    anchors.topMargin: 40
    anchors.bottomMargin: 40
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    opacity: 0

    Behavior on opacity {
        NumberAnimation { duration: 1000 }
     }

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/back_portrait.png"
        smooth: true
    }

    Flickable {
        id: flickable
        anchors {
            fill: parent
            leftMargin: 40
            rightMargin: 40
            topMargin: 60
            bottomMargin: 60
        }
        contentWidth: width
        contentHeight: infoText.height
        clip: true
        Text {
            id: infoText
            width: flickable.width
            color: "black"
            text: "<a href=\"https://projects.forum.nokia.com/wwdaemon\"><font color=\"#000000\">WhoWhere Daemon</font></a><br><br>" +
                  "The WhoWhere Daemon Qt Quick application is a Nokia example and a child of the <a href=\"https://projects.forum.nokia.com/whowhere\"><font color=\"#000000\">WhoWhere</font></a> example. " +
                  "The Daemon is divided into two parts, QML UI Client application and Daemon that is an executable binary without the UI. " +
                  "The Daemon listens to your GPS location and answers location requests from WhoWhere automatically. " +
                  "You can change the settings of the Daemon with this QML UI Client application.<br><br>" +
                  "Sound was loaded from freesound.org under Creative Commons Sampling Plus 1.0 license." +
                  "<br><br>" +
                  "WhoWhere Daemon v1.0"
            wrapMode: Text.WordWrap

            onLinkActivated: {
                Qt.openUrlExternally(link);
            }

        }
    }

    // Exit
    Button {
        id: exit
        buttonId: 1
        width:  50
        height:  50
        y: parent.height - height - 20
        anchors.horizontalCenter: parent.horizontalCenter

        onBtnClicked: {
            main.showInfo(false);
        }
    }

}
